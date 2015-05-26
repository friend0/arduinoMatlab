#ifndef INC_RC_CURVE_H
#define INC_RC_CURVE_H

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Curve.h
** Curve functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <inttypes.h>

#include <InputProcessor.h>
#include <InputSource.h>


namespace rc
{

/*! 
 *  \brief     Class to encapsulate Curve functionality.
 *  \details   This class provides throttle/pitch curves, curve points may be edited as if this were an array.
 *  \author    Daniel van den Ouden
 *  \date      Feb-2012
 *  \copyright Public Domain.
 */
class Curve : public InputProcessor, InputSource
{
public:
	/*! \brief Nameless enum, magic number hiding. */
	enum
	{
		PointCount = 9 //!< Amount of points in the curve
	};
	
	enum DefaultCurve //! Default curves
	{
		DefaultCurve_Linear,     //!< Linear curve [-256 - 256]
		DefaultCurve_HalfLinear, //!< Half linear curve [0 - 256]
		DefaultCurve_V,          //!< V shaped curve [256 - 0 - 256]
		
		DefaultCurve_Count
	};
	
	/*! \brief Constructs a Curve object
	    \param p_curve Default curve to initialize with.
	    \param p_source Input source.
	    \param p_destination Where results should be written to.*/
	Curve(DefaultCurve p_curve = DefaultCurve_Linear,
	      Input p_source = Input_None,
	      Input p_destination = Input_None);
	
	/*! \brief Loads a default curve.
	    \param p_curve Curve to load.*/
	void loadCurve(DefaultCurve p_curve);
	
	/*! \brief Sets a curve point.
	    \param p_point The point to set, range [0 - Curve::PointCount-1].
	    \param p_value The value to set, range [-256 - 256].*/
	void setPoint(uint8_t p_point, int16_t p_value);
	
	/*! \brief Gets a curve point.
	    \param p_point The point to get, range [0 - Curve::PointCount-1].
	    \return The current value, range [-256 - 256].*/
	int16_t getPoint(uint8_t p_point) const;
	
	/*! \brief Array subscript operator, allow direct access to curve points.
	    \param p_point The point to get, range [0 - Curve::PointCount-1].
	    \return Reference to point.*/
	int16_t& operator[](uint8_t p_point);
	
	/*! \brief Array subscript operator, allow direct access to curve points.
	    \param p_point The point to get, range [0 - Curve::PointCount-1].
	    \return Reference to point.*/
	const int16_t& operator[](uint8_t p_point) const;
	
	/*! \brief Applies curve.
	    \param p_value Source value to apply curve to, range [-256 - 256].
	    \return curve applied p_value, range [-256 - 256].*/
	int16_t apply(int16_t p_value) const;
	
	/*! \brief Applies curve to configured input.
	    \return curve applied p_value, range [-256 - 256].*/
	int16_t apply() const;
	
private:
	int16_t m_points[PointCount]; //!< Points
};
/** \example curve_example.pde
 * This is an example of how to use the Curve class.
 */


} // namespace end


#endif // INC_RC_CURVE_H
