/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Gyro.cpp
** Gyro functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <Gyro.h>


namespace rc
{

// Public functions

Gyro::Gyro(Output p_output)
:
OutputSource(p_output),
m_type(Type_Normal),
m_mode(Mode_Normal)
{
	
}


void Gyro::setType(Type p_type)
{
	m_type = p_type;
}
	

Gyro::Type Gyro::getType() const
{
	return m_type;
}

	
void Gyro::setMode(Mode p_mode)
{
	m_mode = p_mode;
}
	

Gyro::Mode Gyro::getMode() const
{
	return m_mode;
}

	
void Gyro::setGain(int8_t p_gain)
{
	m_gain = p_gain;
}
	

int8_t Gyro::getGain() const
{
	return m_gain;
}


Gyro& Gyro::operator = (int8_t p_rhs)
{
	m_gain = p_rhs;
	return *this;
}


Gyro::operator int8_t () const
{
	return m_gain;
}


int16_t Gyro::apply() const
{
	switch (m_type)
	{
		default:
		case Type_Normal:
		{
			int16_t val = m_gain; // range [0 - 100]
			val = val - 50; // range [-50 - 50]
			val *= 512; // range [-25600 - 25600]
			val /= 100; // range [-256 - 256]
			return writeOutputValue(val);
		}
		break;
		
		case Type_AVCS:
		{
			int16_t val = m_gain; // range [0 - 100]
			if (m_mode == Mode_Normal)
			{
				val = -val;
			}
			// range [-100 - 100]
			val *= 256; // range [-25600 - 25600]
			val /= 100; // range [-256 - 256]
			return writeOutputValue(val);
		}
		break;
	}
}


// namespace end
}

