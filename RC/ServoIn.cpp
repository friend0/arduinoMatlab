/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ServoIn.cpp
** Servo Signal Input functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <avr/interrupt.h>
	#include <wiring.h>
#endif

#include <ServoIn.h>
#include <Timer1.h>


namespace rc
{

// Public functions

ServoIn::ServoIn(uint16_t* p_results, uint8_t* p_work, uint8_t p_maxServos)
:
m_maxServos(p_maxServos),
m_high(true),
m_results(p_results),
m_pulseStart(reinterpret_cast<uint16_t*>(p_work)),
m_pulseLength(m_pulseStart + p_maxServos)
{
	
}


void ServoIn::start(bool p_high)
{
	m_high = p_high;
	
	// clean buffers
	for (uint8_t i = 0; i < m_maxServos; ++i)
	{
		m_pulseStart[i]  = 0;
		m_pulseLength[i] = 0;
		m_results[i]     = 0;
	}
	
	// start Timer 1
	rc::Timer1::start();
}


void ServoIn::pinChanged(uint8_t p_servo, bool p_high)
{
	// first things first, get Timer 1 count
	uint8_t oldSREG = SREG;
	cli();
	uint16_t cnt = TCNT1;
	SREG = oldSREG;
	
	if (p_high == m_high)
	{
		// start of pulse, cheat half a microsecond so we can detect errors
		m_pulseStart[p_servo] = (cnt == 0) ? 1 : cnt;
	}
	else
	{
		// end of pulse, clear length on error
		m_pulseLength[p_servo] = (m_pulseStart[p_servo] == 0) ? 0 : (cnt - m_pulseStart[p_servo]);
	}
}


void ServoIn::update()
{
	for (uint8_t i = 0; i < m_maxServos; ++i)
	{
		m_results[i] = m_pulseLength[i] >> 1;
	}
}


// namespace end
}
