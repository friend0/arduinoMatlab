#ifndef INC_RC_CHANNEL_H
#define INC_RC_CHANNEL_H

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Channel.h
** Channel functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <inttypes.h>

#include <OutputProcessor.h>


namespace rc
{

/*! 
 *  \brief     Class to encapsulate Channel transformation functionality.
 *  \details   This class provides channel reverse, end points and subtrim.
 *  \author    Daniel van den Ouden
 *  \date      Feb-2012
 *  \copyright Public Domain.
 */
class Channel : public OutputProcessor
{
public:
	/*! \brief Constructs a Channel object
	    \param p_source Which function output to use as channel input.*/
	Channel(Output p_source = Output_None);
	
	/*! \brief Sets channel reverse.
	    \param p_reverse Whether the channel should be reversed.*/
	void setReverse(bool p_reverse);
	
	/*! \brief Gets channel reverse.
	    \return Whether the channel is reversed.*/
	bool isReversed() const;
	
	/*! \brief Sets subtrim.
	    \param p_subtrim The subtrim, range [-100 - 100].*/
	void setSubtrim(int8_t p_subtrim);
	
	/*! \brief Gets subtrim.
	    \return The subtrim, range [-100 - 100].*/
	int8_t getSubtrim() const;

	/*! \brief Sets end point min.
	    \param p_endPoint The end point of the negative side of the range, range [0 - 140].*/
	void setEndPointMin(uint8_t p_endPoint);
	
	/*! \brief Gets end point min.
	    \return The end point of the negative side of the range, range [0 - 140].*/
	uint8_t getEndPointMin() const;

	/*! \brief Sets end point max.
	    \param p_endPoint The end point of the positive side of the range, range [0 - 140].*/
	void setEndPointMax(uint8_t p_endPoint);
	
	/*! \brief Gets end point max.
	    \return The end point of the positive side of the range, range [0 - 140].*/
	uint8_t getEndPointMax() const;

	/*! \brief Applies channel transformations.
	    \param p_value The normalized value of the channel, range 140% [-358 - 358].
	    \return Normalized channel value, range [-256 - 256].*/
	int16_t apply(int16_t p_value) const;
	
	/*! \brief Applies channel transformations to specified input source.
	    \return Normalized channel value, range [-256 - 256].*/
	int16_t apply() const;
	
private:
	bool     m_reversed; //!< Channel reverse?
	uint8_t  m_epMin;    //!< End point minimum
	uint8_t  m_epMax;    //!< End point maximum
	int8_t   m_subtrim;  //!< Subtrim
};
/** \example channel_example.pde
 * This is an example of how to use the Channel class.
 */


} // namespace end

#endif // INC_RC_CHANNEL_H
