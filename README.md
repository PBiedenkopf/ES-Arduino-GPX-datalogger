## Arduino GPX datalogger
'GPXlogger.ino' is a small GPS logger for Arduino using the TinyGPS++ library. The GPX-files are 
stored on a SD card using a card modul. GPX is an open standard exchange format which is used
by many GPS programs like [Google My Maps](https://www.google.com/maps/d/u/0/). The Logger is 
designed with low power consumtion (< 90 mA) to be used in outdoor activities with a battery. 
Therefore it does not use a display or smartphone connection. To start logging just press the 
button which is indicated with the LED turning on. To stop recording just press the button again.

## Hardware
![Alt text](./wiring.png?raw=true "Title")

Hint: Button1 (Pin 5) is used for thart logging. Button2 (Pin 7) will be used to mark special 
points of interests as waypoints on the current track (not implemented yet).

The following hardware was used in this project:
- [Arduino Nano](https://www.amazon.de/gp/product/B078SBBST6/ref=ppx_yo_dt_b_asin_title_o08_s00?ie=UTF8&psc=1)
- [GPS Modul](https://www.amazon.de/gp/product/B01N38EMBF/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)
- [SD Card Modul](https://www.amazon.de/gp/product/B077MCQS9P/ref=ppx_yo_dt_b_asin_title_o01_s01?ie=UTF8&psc=1)
- Buttons, wires, LED, resistors, breadboard

## License
MIT License

Copyright (c) 2020 Philipp Biedenkopf
