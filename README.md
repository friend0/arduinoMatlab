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

Navigate the the 'arduinoMatlabImproved' directory. 
Find the arduino sketch for setting up serial, and pinging a matlab serial session. Can flash this to the Uno using the Arduino IDE using AVRdude bootloader.

Also find the 'transmitSerial.m' Matlab script. This file sets up a serial port and listens for the Arduino, which ought to be sending a stream of 'A's. When the Uno is detected and serial comm is established, Matlab pauses to wait for Uno to reset, then sends data. 

The state of the code is as follows. I've had some success wensing strings over serial and using Serial.parseInt() on the arduino side, but it seems to choke when sending multiple packets. 

Forgive the messy code, I've been trying a lot of things. It may be interesting to fall back to the 'bitbanged' routines that are commented out on both the Arduino and Matlab sides if we cant get the two to play together using the higher level code we're trying now. 

#Todo
- Need to specify an API for Matlab to interface to
- Need to write the Matlab script - right now I have a minimally operational script which can connect to and grab a few packets from the Arduino
- Need to determine if an op-amp is needed at the output of the Arduino, also consider switching to a 3.3v Arduino