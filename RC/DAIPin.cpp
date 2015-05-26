/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** DIPin.cpp
** Digital input read functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <wiring.h>
#endif

#include <DAIPin.h>
#include <util.h>


namespace rc
{

// Public functions

DAIPin::DAIPin(uint8_t p_pin, Input p_destination)
:
DIPin(p_pin),
InputSource(p_destination),
m_duration(0),
m_time(0),
m_lastTime(0)
{
	
}


void DAIPin::setDuration(uint16_t p_duration)
{
	m_duration = p_duration;
	
	// instantly update, to prevent overflows and such
	if (read())
	{
		m_time = p_duration;
	}
	else
	{
		m_time = 0;
	}
	update();
}


uint16_t DAIPin::getDuration() const
{
	return m_duration;
}


int16_t DAIPin::update()
{
	uint16_t now = static_cast<uint16_t>(millis());
	uint16_t delta = now - m_lastTime;
	m_lastTime = now;
	
	if (read())
	{
		// move up
		if (m_duration == 0)
		{
			return writeInputValue(256);
		}
		
		m_time += delta;
		
		// clamp
		if (m_time > m_duration)
		{
			m_time = m_duration;
		}
	}
	else
	{
		// move down
		if (m_duration == 0)
		{
			return writeInputValue(-256);
		}
		
		// clamp
		if (m_time < delta)
		{
			m_time = 0;
		}
		else
		{
			m_time -= delta;
		}
	}
	return writeInputValue(rc::rangeToNormalized(m_time, m_duration));
}


// namespace end
}
