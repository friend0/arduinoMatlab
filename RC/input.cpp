/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** input.cpp
** Place where the input for various functions are stored
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <input.h>


namespace rc
{

static int16_t s_values[Input_Count] = { 0 };


void setInput(Input p_input, int16_t p_value)
{
	s_values[p_input] = p_value;
}


int16_t getInput(Input p_input)
{
	return s_values[p_input];
}


// namespace end
}
