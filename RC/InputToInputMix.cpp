/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** InputToInputMix.cpp
** Generic input to input mix
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <InputToInputMix.h>
#include <util.h>


namespace rc
{

// Public functions

InputToInputMix::InputToInputMix(int8_t p_mix, bool p_abs, Input p_source, Input p_index)
:
InputProcessor(p_source),
InputModifier(p_index),
m_mix(p_mix),
m_abs(p_abs)
{
	
}


void InputToInputMix::setMix(int8_t p_mix)
{
	m_mix = p_mix;
}


int8_t InputToInputMix::getMix() const
{
	return m_mix;
}


void InputToInputMix::setUseAbs(bool p_abs)
{
	m_abs = p_abs;
}


bool InputToInputMix::getUseAbs() const
{
	return m_abs;
}


int16_t InputToInputMix::apply(int16_t p_master, int16_t p_slave) const
{
	if (m_abs)
	{
		p_master = p_master < 0 ? -p_master : p_master;
	}
	return rc::clamp140(p_slave + rc::mix(p_master, m_mix));
}


void InputToInputMix::apply() const
{
	if (m_index != Input_None && m_source != Input_None)
	{
		rc::setInput(m_index,
		             apply(rc::getInput(m_source),
		                   rc::getInput(m_index)));
	}
}


// namespace end
}
