/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** throttlehold_example.pde
** Demonstrate Throttle Hold functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <AIPin.h>
#include <DIPin.h>
#include <ThrottleHold.h>


// we use A0 as input for the throttle
rc::AIPin g_thrPin(A0, rc::Input_THR);

// we use pin 3 as a hold switch
rc::DIPin g_hldPin(3);

// we can define the throttle hold level in the contructor
// this is the level the throttle will be on when hold is enabled
rc::ThrottleHold g_hold(-256);

void setup()
{
	// nothing to do here
}

void loop()
{
	g_thrPin.read(); // will write results to rc::Input_THR
	// ThrottleHold uses rc::Input_THR as default input
	// you can specify a different input using setIndex()
	// or you can specify some value in the apply function
	
	g_hold.apply(g_hldPin.read());
	// in case the hold pin was high, the result will be -256 (which we specified in the constructor)
	// otherwise it will be whatever g_thrPin.read() wrote.
	// you can get the result using rc::getInput(rc::Input_THR);
	// print it out using Serial to see it in action!
}
