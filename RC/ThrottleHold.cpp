/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ThrottleHold.cpp
** Throttle hold functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <ThrottleHold.h>


namespace rc
{
// Public functions

ThrottleHold::ThrottleHold(int16_t p_throttle, Input p_index)
:
InputModifier(p_index),
m_throttle(p_throttle)
{
	
}


void ThrottleHold::setThrottle(int16_t p_throttle)
{
	m_throttle = p_throttle;
}


int16_t ThrottleHold::getThrottle() const
{
	return m_throttle;
}


int16_t ThrottleHold::apply(bool p_enabled, int16_t p_throttle) const
{
	return p_enabled ? m_throttle : p_throttle;
}


void ThrottleHold::apply(bool p_enabled) const
{
	if (m_index != Input_None)
	{
		rc::setInput(m_index, apply(p_enabled, rc::getInput(m_index)));
	}
}


// namespace end
}
