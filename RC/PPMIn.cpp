/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** PPMIn.cpp
** Pulse Position Modulation Input functionality
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

#include <PPMIn.h>
#include <Timer1.h>


namespace rc
{

// Public functions

PPMIn::PPMIn(uint16_t* p_results, uint8_t* p_work, uint8_t p_maxChannels)
:
m_state(State_Startup),
m_channels(0),
m_pauseLength(8000),
m_timeout(500),
m_results(p_results),
m_work(reinterpret_cast<uint16_t*>(p_work)),
m_maxChannels(p_maxChannels),
m_idx(0),
m_newFrame(false),
m_lastFrameTime(0),
m_lastTime(0),
m_high(false)
{
	
}


void PPMIn::start(bool p_high)
{
	m_high = p_high;
	
	// check if Timer 1 is running or not
	rc::Timer1::start();
}


void PPMIn::setPauseLength(uint16_t p_length)
{
	m_pauseLength = p_length << 1;
}


uint16_t PPMIn::getPauseLength() const
{
	return m_pauseLength >> 1;
}


void PPMIn::setTimeout(uint16_t p_length)
{
	m_timeout = p_length;
}


uint16_t PPMIn::getTimeout() const
{
	return m_timeout;
}


bool PPMIn::isStable() const
{
	return m_state == State_Stable;
}


bool PPMIn::isLost() const
{
	return m_state == State_Lost;
}


uint8_t PPMIn::getChannels() const
{
	return m_channels;
}


void PPMIn::pinChanged(bool p_high)
{
	if (p_high != m_high)
	{
		return;
	}
	
	// first things first, get Timer 1 count
	uint8_t oldSREG = SREG;
	cli();
	uint16_t cnt = TCNT1;
	SREG = oldSREG;
	
	switch (m_state)
	{
	default:
	case State_Startup:
	case State_Lost:
		{
			if (cnt - m_lastTime >= m_pauseLength)
			{
				m_state = State_Listening;
				m_channels = 0;
			}
		}
		break;
	
	case State_Listening:
		{
			if (cnt - m_lastTime >= m_pauseLength)
			{
				m_state = State_Stable;
				m_idx = 0;
				m_newFrame = true;
			}
			else
			{
				if (m_channels < m_maxChannels)
				{
					m_work[m_channels] = cnt - m_lastTime;
				}
				++m_channels;
			}
		}
		break;
	
	case State_Stable:
		{
			if (cnt - m_lastTime >= m_pauseLength)
			{
				if (m_idx == m_channels)
				{
					m_idx = 0;
					m_newFrame = true;
				}
				else
				{
					m_state = State_Confused;
				}
			}
			else
			{
				if (m_idx < m_maxChannels)
				{
					m_work[m_idx] = cnt - m_lastTime;
				}
				++m_idx;
			}
		}
		break;
	}
	m_lastTime = cnt;
}


bool PPMIn::update()
{
	if (m_newFrame)
	{
		m_newFrame = false;
		m_lastFrameTime = static_cast<uint16_t>(millis());
		for (uint8_t i = 0; i < m_channels && i < m_maxChannels; ++i)
		{
			m_results[i] = m_work[i] >> 1;
		}
		return true;
	}
	else if (m_state == State_Stable)
	{
		uint16_t delta = static_cast<uint16_t>(millis()) - m_lastFrameTime;
		if (delta >= m_timeout)
		{
			// signal lost
			m_state = State_Lost;
		}
	}
	return false;
}


// namespace end
}
