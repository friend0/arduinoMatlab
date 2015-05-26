# arduinoMatlab
This code will be used to support communication between RC vehicles and controllers developed in matlab. 

#Motivation
This module was inspired by a need to replace the Endurance PCTx with a platform agnostic solution. Because the PCTx consits only of a relatively dated microcontroller running standard PPM algorithms, it made sense to develop our own solution. 

#How it works

Controller <-> Matlab <-> Arduino <-> DX6i RC Transmitter <-> RC Vehicle

The com link between matlab and the arduino is a simple serial connection. The link supports byte-wise transmissions, but a lightweight protocol, allows for the simple routing of packets and recombination into higher-order integers from one end to the other. 

#How to use
As of now (5/26/15) I have only developed the Arduino sketch to be run on the Uno. This sketch performs some hardware setup, then pings the serial port. 

The Matlab script that will be written, when run, will listen for this ping. Once received, it will send an acknowledge packet. When the Ack packet is reciedved by the uno, it will begin the PPM interrupt routine, and will also run a simple FSM for processing packets form Matlab. 

#Todo
- Need to specify an API for Matlab to interface to
- Need to write the Matlab script - right now I have a minimally operational script which can connect to and grab a few packets from the Arduino
- Need to determine if an op-amp is needed at the output of the Arduino, also consider switching to a 3.3v Arduino