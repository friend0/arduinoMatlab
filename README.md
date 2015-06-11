# arduinoMatlab
This code will be used to support communication between RC vehicles and controllers developed in matlab. 

#Motivation
This module was inspired by a need to replace the Endurance PCTx with a platform agnostic solution. Because the PCTx consits only of a relatively dated microcontroller running standard PPM algorithms, it made sense to develop our own solution. 

#How it works

Controller <-> Matlab <-> Arduino <-> DX6i RC Transmitter <-> RC Vehicle

The com link between matlab and the arduino is a simple serial connection. The link supports byte-wise transmissions, but a lightweight protocol, allows for the simple routing of packets and recombination into higher-order integers from one end to the other. 

#How to use
Clone the repo with:
```
git clone https://github.com/empireryan/arduinoMatlab.git
```

Navigate to the 'arduinoMatlabImproved' directory. This is where the lates code is.

Find the arduino sketch for setting up serial, and pinging a matlab serial session. You can flash this to the Uno using the Arduino IDE using the AVRdude bootloader.

Also find the 'transmitSerial.m' and 'initSerial.m' Matlab scripts. This file sets up a serial port and listens for the Arduino, which ought to be sending a stream of 'A's. When the Uno is detected and serial comm is established, Matlab pauses to wait for Uno to reset, then sends data. 

Right now, the com port is hard-coded into the init script. Change this to the current port that your Arduino is on. make sure to grab the returned serial object from the function so that it is in the workspace.

From here, given that the connection is succesful, we ought to be able to send data. It is important to make sure that the Uno has been flashed, and is waiting to make a serial connection by sending out 'A's. 

Here is an example:

```
ppmValues = [2000,2000,1500,1500,2000,2000];
[s,flag] = initSerial;
transmitSerial(s, ppmValues);
```

Note that I believe the RC will be expecting standard PPM values, from 1000 mS to 2000mS with 1500 being the nominal value. There is currently no protection on this end for values outside of this range.

#Todo
- Add value 'saturation' to out of range ppm values