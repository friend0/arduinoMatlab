/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ppmout_example.pde
** Demonstrate Pulse Position Modulation Output functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <PPMOut.h>
#include <Timer1.h>


#define CHANNELS 4

uint8_t  g_pins[CHANNELS] = {A0, A1, A2, A3}; // Input pins
uint16_t g_input[CHANNELS];                   // Input buffer in microseconds
uint8_t  g_work[PPMOUT_WORK_SIZE(CHANNELS)];  // we need to have a work buffer for the PPMOut class

// PPMOut requires two buffers:
//     Input buffer containing input samples in microseconds
//     Work buffer of ((channels + 1) * 2) elements for internal calculations and frame buffering
// This setup removes any limit on the number of channels you want, and makes sure the library doesn't use more
// memory than it really needs, since the client code supplies the buffers.
rc::PPMOut g_PPMOut(CHANNELS, g_input, g_work, CHANNELS);

void setup()
{
	// Initialize timer1, this is required for all features that use Timer1
	// (PPMIn/PPMOut/ServoIn/ServoOut)
	rc::Timer1::init();
	
	for (uint8_t i = 0;  i < CHANNELS; ++i)
	{
		// set up input pins
		pinMode(g_pins[i], INPUT);
		
		// fill input buffer, convert raw values to microseconds
		g_input[i] = map(analogRead(g_pins[i]), 0, 1024, 1000, 2000);
	}
	
	// initialize PPMOut with some settings
	g_PPMOut.setPulseLength(448);   // pulse length in microseconds
	g_PPMOut.setPauseLength(10448); // length of pause after last channel in microseconds
	// note: this is also called the end of frame, or start of frame, and is usually around 10ms
	
	// start PPMOut, use pin 9 (pins 9 and 10 are preferred)
	g_PPMOut.start(9);
}

void loop()
{
	// update the input buffer
	for (uint8_t i = 0;  i < CHANNELS; ++i)
	{
		// fill input buffer, convert raw values to microseconds
		g_input[i] = map(analogRead(g_pins[i]), 0, 1024, 1000, 2000);
	}
	
	// tell PPMOut there are new values available in the input buffer
	g_PPMOut.update();
}
