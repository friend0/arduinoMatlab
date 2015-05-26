/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** servoout_example.pde
** Demonstrate Servo Signal Output functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <ServoOut.h>
#include <Timer1.h>

#define SERVOS 4

uint8_t  g_pinsIn[SERVOS] = {A0, A1, A2, A3}; // Input pins
uint8_t  g_pinsOut[SERVOS] = {2, 3, 4, 5};    // Output pins
uint16_t g_input[SERVOS];                     // Input buffer for servoOut, microseconds
uint8_t  g_work[SERVOOUT_WORK_SIZE(SERVOS)];  // we need to have a work buffer for the ServoOut class

// ServoOut requires three buffers:
//     Pins buffer containing output pins
//     Input buffer containing input values in microseconds
//     Work buffer of SERVOOUT_WORK_SIZE(SERVOS) elements for internal calculations
// This setup removes any technical limit on the number of servos you want,
// and makes sure the library doesn't use more memory than it really needs,
// since the client code supplies the buffers.
rc::ServoOut g_ServoOut(g_pinsOut, g_input, g_work, SERVOS);

void setup()
{
	// Initialize timer1, this is required for all features that use Timer1
	// (PPMIn/PPMOut/ServoIn/ServoOut)
	rc::Timer1::init();
	
	for (uint8_t i = 0;  i < SERVOS; ++i)
	{
		// set up input pins
		pinMode(g_pinsIn[i], INPUT);
		
		// set up output pins
		pinMode(g_pinsOut[i], OUTPUT);
		
		// put them low
		digitalWrite(g_pinsOut[i], LOW);
		
		// fill input buffer, convert raw values to normalized ones
		g_input[i] = map(analogRead(g_pinsIn[i]), 0, 1024, 1000, 2000);
	}
	g_ServoOut.start();
}

void loop()
{
	// update the input buffer
	for (uint8_t i = 0;  i < SERVOS; ++i)
	{
		// fill input buffer, convert raw values to normalized ones
		g_input[i] = map(analogRead(g_pinsIn[i]), 0, 1024, 1000, 2000);
	}
	
	// tell ServoOut there are new values available in the input buffer
	g_ServoOut.update();
	
	// Just a quick note here to keep in mind
	// If you're having problems getting your servos to work properly, i.e. servos are not responding well
	// or seem to miss their input signal every couple of seconds, then the servos may be disturbing their control signal.
	// This happens especially when you've connected the servos directly to the Arduino and when they are of poor quality.
	// The motor of the servo will cause drops in the voltage of the Arduino which will mess with the generated signal,
	// I've had this happen to me while using some cheap Esky micro servos. If you hook the signal up to a scope you
	// can see voltage drops in the signal every few hundred microseconds while the servo's motor is working.
	// To fix this you need a couple of decoupling capacitors, google for more details ;)
}
