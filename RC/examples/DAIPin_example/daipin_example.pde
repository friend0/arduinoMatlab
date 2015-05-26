/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** daipin_example.pde
** Demonstrate digital analog input pin functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <DAIPin.h>


rc::DAIPin g_pin(3); // we want to use digital pin 3, we can use any digital pin

void setup()
{
	// If our switch is mounted the wrong way,
	// we can simply reverse the input like this
	// g_pin.setReverse(true);
	
	// DAIPin acts like an analog pin but takes digital input
	// The result of a read will be a value between -256 and 256
	// A low input will result in -256, a high input in 256 (without reversing ofcourse)
	// It's also possible to set a duration to specify how long the transition between
	// -256 and 256 will take (in milliseconds).
	// for example:
	g_pin.setDuration(1000); // 1 second
	// now if the input changes it will take one second before the new value is reached
	// for example, if the input signal switches from low to high, the output of the read
	// function will be as follows:
	// after 0 ms: -256
	// after 250 ms: -128
	// after 500 ms: 0
	// after 750 ms: 128
	// after 1000 ms: 256
	
	// just like AIPin, we can tell the DAIPin to write its output to the input system
	g_pin.setDestination(rc::Input_FLP);
}

void loop()
{
	// all we need to do is call the read function and we get the pin value,
	int16_t value = g_pin.read();
	
	// and since we're using the input system, we can also access the result like this:
	value = rc::getInput(rc::Input_FLP);
	
	// That's all!
}
