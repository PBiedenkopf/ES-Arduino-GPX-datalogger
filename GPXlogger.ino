#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>


// objects for gps
const uint8_t chipSelect = 10;
TinyGPSPlus gps;
SoftwareSerial ss(4, 3); // ss(RXpin, TXpin); serial connection to gps-device 

// objects for sd card
File gpx;
int fileID = 0;
char GPXFILE[13];
unsigned long t_start;   // timer for sampling
const int PROGMEM sample = 1000; // sample time [ms]

// objects for button and led
const uint8_t buttonPin = 5;
const uint8_t buttonPinWpt = 7;
const uint8_t ledPin = 6;
bool b_gpx_tracking = false; // flag for starting the gpx logging
bool b_gpx_init = false; // flag that indicates if gpx file is initialized



void setup() {
  // select baud rates
  Serial.begin(9600);
  ss.begin(9600);

  // initialize SD card
  Serial.print(F("Initializing SD card ..."));
  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card initialization failed!"));
    while (1);
  }
  Serial.println("Card initialized");

  // initialize start button
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPinWpt, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  // start timer the first time
  t_start = millis();
}

// ------------------------------------------------------------------------
void loop(){
  // Start/stop GPX record on button state IF serial connection is valid
  if (digitalRead(buttonPin) == HIGH && ss.available() > 0) {
    if (!b_gpx_tracking) {
      gps.encode(ss.read());
      // Define non-existing filename by eventually increasing the ID
      while (1) {
          sprintf(GPXFILE, "track%02d.gpx", fileID);
          if (!SD.exists(GPXFILE)) {
            break;
          }
          else {
            fileID++;
          }
        }
      Serial << F("Current gpx file: ") << GPXFILE << "\n";
      initGPX(); // initialize gpx file header
      b_gpx_tracking = true;
      b_gpx_init = true; // gpx file is initialized now
      digitalWrite(ledPin, HIGH);
      delay(300);
    }
    else {
      b_gpx_tracking = false;
      digitalWrite(ledPin, LOW);
      delay(300);
    }
  }
  
  // if gps data is available via serial connection
  if (ss.available() > 0) {
    gps.encode(ss.read()); // read gps data

    // log data only if sample time is reached
    if (millis() - t_start > sample) {
      // log a new track point to gpx file
      if (b_gpx_tracking) {
        addTrackpt();
      }
      // complete gpx file if recording is stopped after it was executed once
      if (b_gpx_init && !b_gpx_tracking) {
        completeGPX();
        b_gpx_init = false; // current file is completed and not initialized anymore
        digitalWrite(ledPin, LOW); // turn of record-led
      }

      // reset timer for next iteration
      t_start = millis();
    }
  }  
}
// ------------------------------------------------------------------------

/// @brief Initialize the gpx file structure
void initGPX() {
      Serial.println(F("Initializing GPX file ..."));
      gpx = SD.open(GPXFILE, FILE_WRITE);

      if (gpx) {
        // header
        gpx.print(F("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"));

        // start of gpx file body
        gpx.print(F("<gpx version=\"1.1\" creator=\"P. Biedenkopf\" xmlns=\"http://www.topografix.com/GPX/1/1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"));

        // metadata
        gpx.print(F("  <metadata>\n"));
        gpx << "    <name>" << GPXFILE << "</name>\n";
        gpx.print(F("    <desc>Tracked with Arduino Nano</desc>\n"));
        //gpx.print(F("    <author>\n"));
        //gpx.print(F("      <name>P. Biedenkopf</name>\n"));
        //gpx.print(F("      <email>ph.bied13@gmail.com</email>\n"));
        //gpx.print(F("    </author>\n"));
        gpx.print(F("  </metadata>\n"));

        // start track and track segment (only single segmented tracks possible)
        gpx.print(F("  <trk>\n"));
        gpx << "    <name>" << GPXFILE << "</name>\n";
        gpx.print(F("    <desc>Tracked with Arduino Nano</desc>\n"));
        gpx.print(F("    <trkseg>\n"));
        
        gpx.close();
      }
      else {
        Serial.println(F("Error opening gpx-file"));
      }
}

/// @brief Create a trackpoint in gpx syntax
void addTrackpt() {
      Serial.println(F("Adding Trackpoint ..."));
      gpx = SD.open(GPXFILE, FILE_WRITE);

      if (gpx) {
        gpx.print(F("      <trkpt lat=\""));
        gpx.print(gps.location.lat(), 8);
        gpx.print(F("\" lon=\""));
        gpx.print(gps.location.lng(), 8);
        gpx.println("\">");
        gpx << "        <ele>" << gps.altitude.meters() << "</ele>\n";
        char time[21];
        sprintf(time, "%04d-%02d-%02dT%02d:%02d:%02dZ",gps.date.year(),gps.date.month(),gps.date.day(),gps.time.hour(),gps.time.minute(),gps.time.second());
        Serial.println(time); // test
        gpx << "        <time>" << time << "</time>\n";
        gpx << "        <sat>" << gps.satellites.value() << "</sat>\n";
        gpx << "        <hdop>" << gps.hdop.value() << "</hdop>\n";
        gpx << "      </trkpt>\n";
        gpx.close();
      }
      else {
        Serial.println(F("Error opening gpx-file"));
      }
}

/// @brief Create a waypoint in gpx syntax
/// @info TEST!: Waypoints have to be outside of the track declaration. Therefore they are stored in a temp file and get copied at the end of the definition
void addWaypt() {
      Serial.println(F("Adding Waypoint ..."));
      gpx = SD.open(GPXFILE, FILE_WRITE);

      if (gpx) {
        gpx << "    <wpt lat=\"" << (gps.location.lat(),6) << "\" lon=\"" << (gps.location.lng(),6) << "\">\n";
        gpx << "    <ele>" << gps.altitude.meters() << "</ele>\n";
        char time[21];
        sprintf(time, "/%04d-%02d-%02dT02d:%02d:%02Z",gps.date.year(),gps.date.month(),gps.date.day(),gps.time.hour(),gps.time.minute(),gps.time.second());
        Serial.println(time); // test
        gpx << "    <time>" << time << "</time>\n";
        gpx << "    <sat>" << gps.satellites.value() << "</sat>\n";
        gpx << "    <hdop>" << gps.hdop.value() << "</hdop>\n";
        gpx << "    </wpt>\n";
        gpx.close();
      }
      else {
        Serial.println(F("Error opening gpx-file"));
      }
}

/// @brief Conclude the gpx file structure
void completeGPX() {
      Serial.println(F("Completing GPX file ..."));
      gpx = SD.open(GPXFILE, FILE_WRITE);

      if (gpx) {
        // complete recorded track, track segment and gpx file body
        gpx.print(F("    </trkseg>\n"));
        gpx.print(F("  </trk>\n"));
        gpx.print(F("</gpx>\n"));
        gpx.close();
      }
      else {
        Serial.println(F("Error opening gpx-file"));
      }
}


/// @brief Enable C++ style printing templates
template<class T> inline Print &operator <<(Print &str, T arg) {
  str.print(arg);
  return str;
}
