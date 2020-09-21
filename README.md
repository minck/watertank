This is a server that implements a simple simulation that is described in PI Diagram.

![PI Diagram](/tank.png)

Building and running:
```
mkdir build
cd build
cmake ..
make
./watertank
```

Connect with UaExpert to http://localhost:4840, to see the simulation.

![UAExpert tree](/uaexpert.png)

Simulation:
* Simulation has two valves and a water tank. 
* Tank size and water level can be viewed.
* Opening Valve In flows water in the tank.
* Valve Out value changes randomly (using perlin noise) to simulate water going to the process.
* Calling methods Open and Close in Valves allows opening and closing them or change value to open a given amount.
