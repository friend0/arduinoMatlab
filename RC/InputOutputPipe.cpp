/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** InputOutputPipe.cpp
** Class copying from input directly to output
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <InputOutputPipe.h>


namespace rc
{

// Public functions

InputOutputPipe::InputOutputPipe(Input p_source, Output p_destination)
:
InputProcessor(p_source),
OutputSource(p_destination)
{
	
}


void InputOutputPipe::apply() const
{
	if (m_source != Input_None)
	{
		writeOutputValue(rc::getInput(m_source));
	}
}


// namespace end
}
