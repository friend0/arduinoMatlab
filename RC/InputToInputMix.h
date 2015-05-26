#ifndef INC_RC_INPUTTOINPUTMIX_H
#define INC_RC_INPUTTOINPUTMIX_H

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** InputToInputMix.h
** Generic input to input mix
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <inttypes.h>

#include <InputProcessor.h>
#include <InputModifier.h>


namespace rc
{

/*! 
 *  \brief     Class which mixes one input to another.
 *  \details   This class provides basic input to input mixing.
 *  \author    Daniel van den Ouden
 *  \date      Nov-2012
 *  \copyright Public Domain.
 */
class InputToInputMix : public InputProcessor, InputModifier
{
public:
	/*! \brief Constructs an InputToInputMix object
	    \param p_mix The amount of mix to use, range [-100 - 100].
	    \param p_abs Whether to use absolute values for master.
	    \param p_source Input source: master.
	    \param p_index Index the mix should be applied to: slave.*/
	InputToInputMix(int8_t p_mix = 0,
	                bool p_abs = false,
	                Input p_source = Input_None,
	                Input p_index = Input_None);
	
	/*! \brief Sets the amount of mix.
	    \param p_mix The amount of mix to set, range [-100 - 100].*/
	void setMix(int8_t p_mix);
	
	/*! \brief Gets the amount of mix.
	    \return The current amount of mix, range [-100 - 100].*/
	int8_t getMix() const;
	
	/*! \brief Sets whether to use absolute values for master.
	    \param p_abs Whether to use absolute values for master.*/
	void setUseAbs(bool p_abs);
	
	/*! \brief Gets whether to use absolute values for master.
	    \return Whether to use absolute values for master.*/
	bool getUseAbs() const;
	
	/*! \brief Applies mix.
	    \param p_master Value that will be mixed, range [-358 - 358].
		\param p_slave Value that mix will be applied to, range [-358 - 358].
	    \return p_master mixed into p_slave, range [-358 - 358] (clamped).*/
	int16_t apply(int16_t p_master, int16_t p_slave) const;
	
	/*! \brief Applies mix to configured input.*/
	void apply() const;
	
private:
	int8_t m_mix; //!< Amount of mix to apply
	bool   m_abs; //!< Whether to use absolute values for master.
};
/** \example inputtoinputmix_example.pde
 * This is an example of how to use the InputToInputMix class.
 */


} // namespace end


#endif // INC_RC_INPUTTOINPUTMIX_H
