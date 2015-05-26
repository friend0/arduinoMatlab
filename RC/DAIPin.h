#ifndef INC_RC_DAIPIN_H
#define INC_RC_DAIPIN_H

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** DAIPin.h
** Digital input pin which acts like an analog pin
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <DIPin.h>
#include <InputSource.h>


namespace rc
{

/*! 
 *  \brief     Class to encapsulate digital input functionality.
 *  \details   This class provides functionality for reading digital input.
 *  \author    Daniel van den Ouden
 *  \date      Nov-2012
 *  \copyright Public Domain.
 */
class DAIPin : public DIPin, public InputSource
{
public:
	/*! \brief Constructs an DAIPin object.
	    \param p_pin The hardware pin to use.
	    \param p_destination Index to use as destination for the input.*/
	DAIPin(uint8_t p_pin, Input p_destination = Input_None);
	
	/*! \brief Sets the time it takes to transition between states.
	    \param p_time The duration of the transition in milliseconds, range [0 - 10000] (instant - 10 sec).*/
	void setDuration(uint16_t p_time);
	
	/*! \brief Gets the time it takes to transition between states.
	    \return The duration of the transition in milliseconds, range [0 - 10000] (instant - 10 sec).*/
	uint16_t getDuration() const;
	
	/*! \brief Updates internal state.
	    \return Current position, normalized [-256 - 256].*/
	int16_t update();
	
private:
	uint16_t m_duration; //!< Time which it takes to transition in milliseconds (0 = instant)
	uint16_t m_time;     //!< Current position in timeline.
	uint16_t m_lastTime; //!< Time at which previous update was called.
};
/** \example daipin_example.pde
 * This is an example of how to use the DAIPin class.
 */
 

} // namespace end

#endif // INC_RC_DAIPIN_H
