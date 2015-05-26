#ifndef INC_RC_DIPIN_H
#define INC_RC_DIPIN_H

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** DIPin.h
** Digital input pin functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <inttypes.h>


namespace rc
{

/*! 
 *  \brief     Class to encapsulate digital input functionality.
 *  \details   This class provides functionality for reading digital input.
 *  \author    Daniel van den Ouden
 *  \date      Feb-2012
 *  \copyright Public Domain.
 */
class DIPin
{
public:
	/*! \brief Constructs an DIPin object.
	    \param p_pin The hardware pin to use.*/
	DIPin(uint8_t p_pin);
	
	/*! \brief Sets the hardware pin to use.
	    \param p_pin The hardware pin to use.*/
	void setPin(uint8_t p_pin);
	
	/*! \brief Gets the hardware pin.
	    \return Current hardware pin.*/
	uint8_t getPin() const;
	
	/*! \brief Assignment operator, sets hardware pin.
	    \param p_pin The hardware pin to use.
	    \return Reference to DIPin object.*/
	DIPin& operator = (uint8_t p_pin);
	
	/*! \brief Cast operator, casts object to uint8_t.
	    \return Current hardware pin.*/
	operator uint8_t () const;
	
	/*! \brief Sets whether the input should be reversed or not.
	    \param p_reverse Whether to reverse the input or not.*/
	void setReverse(bool p_reverse);
	
	/*! \brief Gets whether the input is reversed.
	    \return Whether the input is reversed or not.*/
	bool isReversed() const;
	
	/*! \brief Reads and processes.
	    \return Processed value.*/
	bool read() const;
	
private:
	uint8_t  m_pin;      //!< Hardware pin.
	bool     m_reversed; //!< Input reverse.
};
/** \example dipin_example.pde
 * This is an example of how to use the DIPin class.
 */
 

} // namespace end

#endif // INC_RC_DIPIN_H
