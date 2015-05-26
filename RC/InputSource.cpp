/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** InputSource.cpp
** Base class for classes acting as a source of input
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <InputSource.h>


namespace rc
{

// Public functions

void InputSource::setDestination(Input p_index)
{
	m_destination = p_index;
}


Input InputSource::getDestination() const
{
	return m_destination;
}


// Protected functions

InputSource::InputSource(Input p_index)
:
m_destination(p_index)
{
	
}


int16_t InputSource::writeInputValue(int16_t p_value) const
{
	if (m_destination != Input_None)
	{
		rc::setInput(m_destination, p_value);
	}
	return p_value;
}


// namespace end
}
