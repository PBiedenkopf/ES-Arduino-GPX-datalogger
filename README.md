## Crankshaft as rotary transducer

'Calculate_eigenfrequency.m' is a Matlab script for analytical modal analysis of rotary transducers. 
It was used to calculate the torsional eigenfrequencies of a modified Honda CBR600 RR crankshaft for 
a Formula Student car. The model for crankshaft consits of a six-mass-system and only has rotational degrees 
of freedom. The model neglects the damping of the system.

![Alt text](./picture/analogous_model.png?raw=true "Title")

The contained stiffness and inertia values refer to the original Honda crankshaft and are estimated 
using an accurate 3D-scan CAD model and FEA simulation. The script calculates the eigenfrequencies and plots an so-called
'image shaft' for each eigenmode. The model was validated with an  modal analysis using FEA simulation and additional
measurements using the impuls hammer method with acceptable results. The difference to the results of the experimental modal 
analysis and the FEA analysis was about 10% for the first eigenmode in this case. The main difficulty is the calculation of the 
stiffness values because they have to be very precise regarding the high impact on the results. There are empirical calculation 
methods as alternatives to the FEA analysis. For more information refer to the methods of Carter and Tuplin in [Knütscher2006](https://books.google.de/books?id=UANwAAAACAAJ&dq=vogel+k%C3%BCntscher+kraftfahrzeugmotoren+2006&hl=de&sa=X&ved=2ahUKEwjG1L3y-MfsAhWjoXEKHXv7Ab4Q6AEwAHoECAAQAg). 

Note that this analysis only handles the torsional eigenfrequencies and no bending eigenmodes are calculated! 


## Usage
Within the top of the script the necessary parameters have to be defined. Nf defines the number of visualized
eigenmodes, OrdPlt defines the highest order in order diagram and OrdStp is the stepsize for the order diagram. 
The stepsize should be either 0.5 or 1. Ji and ci define the inertia and stiffness values of the crankshaft. 

```Matlab
%% Parameter definition
Nf = 2; % Numer of evaluated eigenfrequencies
OrdPlt = 10; % Highest order for visualization
OrdStp = 0.5; % Order steps in diagram

% Moment of inertia [kg*m^2]
J1 = 0.0000199; 
J2 = 0.0016958;
J3 = 0.0021595;
J4 = 0.0018157;
J5 = 0.0012602;
J6 = 0.0000154;

% Stiffness values [Nm/°]
c1_deg = 3014;
c2_deg = 2562;
c3_deg = 2430;
c4_deg = 2636;
c5_deg = 2562;
```
To deal with systems containing more or less mass elements, the inertia and stiffness matricies can simply be extended or simplified.
The six-mass-system is evaluated by solving the eigenvalue problem with the polyeig function. 
```Matlab
[x, e, s] = polyeig(C, D, M);
```

The calculation of the relative amplitudes and the plotted 'image shaft' refers to [Knütscher2006](https://books.google.de/books?id=UANwAAAACAAJ&dq=vogel+k%C3%BCntscher+kraftfahrzeugmotoren+2006&hl=de&sa=X&ved=2ahUKEwjG1L3y-MfsAhWjoXEKHXv7Ab4Q6AEwAHoECAAQAg).
It shows the eigenmode-specific relative amplitude for each mass of the system. In the following you can see the first two 
torsional eigenmodes of the Honda crankshaft with the corresponding frequencies. 
![Alt text](./picture/image_shaft.png?raw=true "Title")

The eigenfrequencies are plotted in an order diagram to visulize potential resonances.
![Alt text](./picture/order_diagram.png?raw=true "Title")

## License
MIT License

Copyright (c) 2020 Philipp Biedenkopf
