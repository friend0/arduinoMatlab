/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** servoin_example.pde
** Demonstrate Servo Signal Input functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <ServoIn.h>
#include <Timer1.h>


#define SERVOS 4

uint16_t g_values[SERVOS];                    // output buffer for ServoIn
uint8_t  g_workIn[SERVOIN_WORK_SIZE(SERVOS)]; // we need to have a work buffer for the ServoIn class

rc::ServoIn g_ServoIn(g_values, g_workIn, SERVOS);

void setup()
{
	// Initialize timer1, this is required for all features that use Timer1
	// (PPMIn/PPMOut/ServoIn/ServoOut)
	rc::Timer1::init();
	
	// We use pin 8-11 as Servo input pins
	pinMode(8, INPUT);
	pinMode(9, INPUT);
	pinMode(10, INPUT);
	pinMode(11, INPUT);
	
	// We use pin change interrupts to detect changes in the signal
	// If you're unfamiliar with how this works, please look up some
	// article or tutorial on the subject.
	
	// only allow pin change interrupts for PB0-3 (digital pins 8-11)
	PCMSK0 = (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
	
	// enable pin change interrupt 0
	PCICR = (1 << PCIE0);
	
	// start listening
	g_ServoIn.start();
}

void loop()
{
	// update incoming values
	g_ServoIn.update();
	
	// handle servo values here, stored in g_values
}


// Interrupt handling code below, this needs cleaning

static uint8_t lastB = 0;

// Pin change port 0 interrupt
ISR(PCINT0_vect)
{
	// we need to call the ServoIn ISR here, keep code in the ISR to a minimum!
	uint8_t newB = PINB;
	uint8_t chgB = newB ^ lastB; // bitwise XOR will set all bits that have changed
	lastB = newB;
	
	// has any of the pins changed?
	if (chgB)
	{
		// find out which pin has changed
		if (chgB & _BV(0))
		{
			g_ServoIn.pinChanged(0, newB & _BV(0));
		}
		if (chgB & _BV(1))
		{
			g_ServoIn.pinChanged(1, newB & _BV(1));
		}
		if (chgB & _BV(2))
		{
			g_ServoIn.pinChanged(2, newB & _BV(2));
		}
		if (chgB & _BV(3))
		{
			g_ServoIn.pinChanged(3, newB & _BV(3));
		}
	}
}
