/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** dipin_example.pde
** Demonstrate digital input pin functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <DIPin.h>


rc::DIPin g_pin(3); // we want to use digital pin 3, we can use any digital pin

void setup()
{
	// If our switch is mounted the wrong way,
	// we can simply reverse the input
	g_pin.setReverse(true);
}

void loop()
{
	// all we need to do is call the read function and we get the pin value,
	// reversed if needed
	bool value = g_pin.read();
	
	// That's all!
}
