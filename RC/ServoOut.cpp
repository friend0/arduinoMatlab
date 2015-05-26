/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ServoOut.cpp
** Servo Signal Output functionality
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
#include <pins_arduino.h>

#include <ServoOut.h>
#include <Timer1.h>


namespace rc
{

ServoOut* ServoOut::s_instance = 0;


// Public functions

ServoOut::ServoOut(const uint8_t* p_pins, const uint16_t* p_values, uint8_t* p_work, uint8_t p_maxServos)
:
m_pauseLength(10000),
m_pins(p_pins),
m_values(p_values),
m_timings(reinterpret_cast<volatile uint16_t*>(p_work)),
m_ports(reinterpret_cast<volatile uint8_t*>(m_timings + p_maxServos + 1)),
m_masks(m_ports + p_maxServos + 1),
m_maxServos(p_maxServos),
m_activePort(0),
m_activeMask(0),
m_nextPort(0),
m_nextMask(0),
m_idx(0)
{
	s_instance = this;
}


void ServoOut::start()
{
	// set initial values
	update(true);
	
	// stop timer 1
	rc::Timer1::stop();
	
	// disable compare match B interrupts
	rc::Timer1::setCompareMatch(false, false);
	
	// set compare value (first, we wait)
	OCR1B = TCNT1 + (m_pauseLength << 1);
	
	// enable timer output compare match B interrupts
	rc::Timer1::setCompareMatch(true, false, ServoOut::handleInterrupt);
	
	// start the timer
	rc::Timer1::start();
}


void ServoOut::setPauseLength(uint16_t p_length)
{
	m_pauseLength = p_length;
}


uint16_t ServoOut::getPauseLength() const
{
	return m_pauseLength;
}


void ServoOut::update(bool p_pinsChanged)
{
	uint16_t remainingTime = m_pauseLength;
	uint8_t idx = 0;
	
	for (uint8_t i = 0; i < m_maxServos; ++i)
	{
		if (m_pins[i] != 0 && m_values[i] != 0)
		{
			if (p_pinsChanged)
			{
				uint8_t mask = digitalPinToBitMask(m_pins[i]);
				uint8_t port = digitalPinToPort(m_pins[i]);
				volatile uint8_t* out = portInputRegister(port);
				
				TIMSK1 &= ~(1 << OCIE1B);
				m_timings[idx] = m_values[i] << 1;
				m_ports[idx] = static_cast<uint8_t>(reinterpret_cast<uint16_t>(out) & 0xFF);
				m_masks[idx] = mask;
				TIMSK1 |= (1 << OCIE1B);
			}
			else
			{
				TIMSK1 &= ~(1 << OCIE1B);
				m_timings[idx] = m_values[i] << 1;
				TIMSK1 |= (1 << OCIE1B);
			}
			
			if (remainingTime < m_values[i])
			{
				remainingTime = 0;
			}
			else
			{
				remainingTime -= m_values[i];
			}
			
			++idx;
		}
	}
	
	TIMSK1 &= ~(1 << OCIE1B);
	m_timings[idx] = remainingTime << 1;
	m_ports[idx] = 0;
	m_masks[idx] = 0;
	TIMSK1 = (1 << OCIE1B);
}


void ServoOut::handleInterrupt()
{
	if (s_instance != 0)
	{
		s_instance->isr();
	}
}


// Private functions

void ServoOut::isr()
{
	// Interrupt Service Routine
	// Needs to be as short and fast as possible
	// But above all, the time spend between the start of the interrupt and the changing of pin values should be
	// as constant as possible, to get the most accurate timings possible.
	
	if (m_activePort != 0)
	{
		// toggle active port (turn it off)
		*m_activePort |= m_activeMask;
	}
	if (m_nextPort != 0)
	{
		// toggle new port (turn it on)
		*m_nextPort |= m_nextMask;
	}
	
	// update compare register
	OCR1B += m_timings[m_idx];
	
	// update active
	m_activePort = m_nextPort;
	m_activeMask = m_nextMask;
	
	// update index
	++m_idx;
	if (m_idx > m_maxServos || m_timings[m_idx] == 0)
	{
		m_idx = 0;
	}
	
	// get next port and mask
	m_nextPort = reinterpret_cast<volatile uint8_t*>(m_ports[m_idx]);
	m_nextMask = m_masks[m_idx];
}


// namespace end
}
