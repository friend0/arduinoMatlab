/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Curve.cpp
** Curve functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <avr/pgmspace.h>

#include <Curve.h>


namespace rc
{

// NOTE: this may give a warning:
// warning: only initialized variables can be placed into program memory area
// this may be safely ignored, it's a known compiler bug in arvgcc (which won't happen for .c files)
static const int16_t PROGMEM sc_defaults[Curve::DefaultCurve_Count][Curve::PointCount] =
{
	{-256, -192, -128, -64,   0,  64, 128, 192, 256}, // DefaultCurve_Linear
	{   0,   32,   64,  96, 128, 160, 192, 224, 256}, // DefaultCurve_HalfLinear
	{ 256,  192,  128,  64,   0,  64, 128, 192, 256}  // DefaultCurve_V
};


// Public functions

Curve::Curve(DefaultCurve p_curve, Input p_source, Input p_destination)
:
InputProcessor(p_source),
InputSource(p_destination)
{
	loadCurve(p_curve);
}


void Curve::loadCurve(DefaultCurve p_curve)
{
	memcpy_P(m_points, sc_defaults[p_curve], sizeof(int16_t) * PointCount);
}


void Curve::setPoint(uint8_t p_point, int16_t p_value)
{
	if (p_point < PointCount)
	{
		m_points[p_point] = p_value;
	}
}


int16_t Curve::getPoint(uint8_t p_point) const
{
	if (p_point < PointCount)
	{
		return m_points[p_point];
	}
	return 0;
}


int16_t& Curve::operator[](uint8_t p_index)
{
	return m_points[p_index];
}


const int16_t& Curve::operator[](uint8_t p_index) const
{
	return m_points[p_index];
}


int16_t Curve::apply(int16_t p_value) const
{
	p_value += 256; // range [0 - 512]
	int16_t index = p_value >> 6;   // divide by 64, range [0 - 8]
	int16_t rem   = p_value & 0x3F; // remainder of division
	
	// linear interpolation on curve values
	int16_t lowval = index >= 8 ? m_points[8] : m_points[index];
	++index;
	int16_t highval = index >= 8 ? m_points[8] : m_points[index];
	
	lowval  = lowval * (64 - rem);
	highval = highval * rem;
	
	return writeInputValue((lowval + highval) >> 6); // weighted average
}


int16_t Curve::apply() const
{
	if (m_source != Input_None)
	{
		return apply(rc::getInput(m_source));
	}
	return 0;
}


// namespace end
}
