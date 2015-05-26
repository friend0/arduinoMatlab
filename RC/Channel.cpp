/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Channel.cpp
** Channel functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <Channel.h>


namespace rc
{

// Public functions

Channel::Channel(Output p_source)
:
OutputProcessor(p_source),
m_reversed(false),
m_epMin(100),
m_epMax(100),
m_subtrim(0)
{
	
}


void Channel::setReverse(bool p_reversed)
{
	m_reversed = p_reversed;
}
	

bool Channel::isReversed() const
{
	return m_reversed;
}

	
void Channel::setSubtrim(int8_t p_subtrim)
{
	m_subtrim = p_subtrim;
}
	

int8_t Channel::getSubtrim() const
{
	return m_subtrim;
}


void Channel::setEndPointMin(uint8_t p_endPoint)
{
	m_epMin = p_endPoint;
}
	

uint8_t Channel::getEndPointMin() const
{
	return m_epMin;
}


void Channel::setEndPointMax(uint8_t p_endPoint)
{
	m_epMax = p_endPoint;
}
	

uint8_t Channel::getEndPointMax() const
{
	return m_epMax;
}


int16_t Channel::apply(int16_t p_value) const
{
	// apply subtrim
	p_value += m_subtrim;

	// apply endpoints
	uint8_t ep = (p_value > 0) ? m_epMax : m_epMin;
	
	// we're running the risk of overflows here, so add a bit of precision
	bool neg = p_value < 0;
	uint16_t val = static_cast<uint16_t>(neg ? (-p_value) : p_value);
	val = (val * ep) / 140;
	
	// clamp values
	if (val > 256) val = 256;
	p_value = neg ? -static_cast<int>(val) : static_cast<int>(val);
	
	// apply channel reverse
	return m_reversed ? -p_value : p_value;
}


int16_t Channel::apply() const
{
	return apply(rc::getOutput(m_source));
}


// namespace end
}

