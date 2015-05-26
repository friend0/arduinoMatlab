/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** inputtoinputmix_example.pde
** Demonstrate Input to Input mixes
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <AIPin.h>
#include <InputToInputMix.h>

// Use A0 as analog input for aileron
rc::AIPin g_pin(A0, rc::Input_AIL);

// specify an aileron to rudder mix
// we use 25% mix (25% of aileron input will be added to rudder input)
// we don't use ABS, this would cause the input to be made absolute (negative input becomes positive)
// we use Input_AIL as master (input)
// we use Input_RUD as slave (destination)
rc::InputToInputMix g_ailToRud(25, false, rc::Input_AIL, rc::Input_RUD);

void setup()
{
	// nothing to do here!
}

void loop()
{
	g_pin.read(); // input from A0 will be placed in Input_AIL
	
	// This is important!
	rc::setInput(rc::Input_RUD, 0);
	// because we don't use any input for rudder, we need to reset it here
	// otherwise aileron will be added in every call of loop(), which would
	// quickly result in rudder being either extremely positive or extremely negative
	
	g_ailToRud.apply(); // apply aileron to rudder mix
	
	// now we can see the results using
	// rc::getInput(rc::Input_AIL);
	// rc::getInput(rc::Input_RUD);
	// give it a try and print it out on the serial port!
}
