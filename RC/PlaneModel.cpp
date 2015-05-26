/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** PlaneModel.cpp
** Wing types, rudder types and tail types.
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** -------------------------------------------------------------------------*/

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <wiring.h>
#endif

#include <input.h>
#include <PlaneModel.h>
#include <util.h>


namespace rc
{

// Public functions

PlaneModel::PlaneModel()
:
m_wing(WingType_Tailed),
m_tail(TailType_Normal),
m_rudder(RudderType_Normal),
m_ailerons(AileronCount_1),
m_flaps(FlapCount_0),
m_brakes(BrakeCount_0),
m_ailDiff(0),
m_wingletDiff(0),
m_elevonAil(50),
m_elevonEle(50),
m_ailevator(50),
m_ailevatorDiff(0),
m_vtailEle(50),
m_vtailRud(50)
{
	
}


void PlaneModel::setWingType(WingType p_type)
{
	m_wing = p_type;
}


PlaneModel::WingType PlaneModel::getWingType() const
{
	return m_wing;
}


void PlaneModel::setTailType(TailType p_type)
{
	m_tail = p_type;
}


PlaneModel::TailType PlaneModel::getTailType() const
{
	return m_tail;
}


void PlaneModel::setRudderType(RudderType p_type)
{
	m_rudder = p_type;
}


PlaneModel::RudderType PlaneModel::getRudderType() const
{
	return m_rudder;
}


void PlaneModel::setAileronCount(AileronCount p_count)
{
	m_ailerons = p_count;
}


PlaneModel::AileronCount PlaneModel::getAileronCount() const
{
	return m_ailerons;
}


void PlaneModel::setFlapCount(FlapCount p_count)
{
	m_flaps = p_count;
}


PlaneModel::FlapCount PlaneModel::getFlapCount() const
{
	return m_flaps;
}


void PlaneModel::setBrakeCount(BrakeCount p_count)
{
	m_brakes = p_count;
}


PlaneModel::BrakeCount PlaneModel::getBrakeCount() const
{
	return m_brakes;
}


void PlaneModel::setAileronDifferential(int8_t p_rate)
{
	m_ailDiff = p_rate;
}


int8_t PlaneModel::getAileronDifferential() const
{
	return m_ailDiff;
}


void PlaneModel::setWingletDifferential(int8_t p_rate)
{
	m_wingletDiff = p_rate;
}


int8_t PlaneModel::getWingletDifferential() const
{
	return m_wingletDiff;
}


void PlaneModel::setElevonAileronMix(int8_t p_rate)
{
	m_elevonAil = p_rate;
}


int8_t PlaneModel::getElevonAileronMix() const
{
	return m_elevonAil;
}


void PlaneModel::setElevonElevatorMix(int8_t p_rate)
{
	m_elevonEle = p_rate;
}


int8_t PlaneModel::getElevonElevatorMix() const
{
	return m_elevonEle;
}


void PlaneModel::setAilevatorMix(int8_t p_rate)
{
	m_ailevator = p_rate;
}


int8_t PlaneModel::getAilevatorMix() const
{
	return m_ailevator;
}


void PlaneModel::setAilevatorDifferential(int8_t p_rate)
{
	m_ailevatorDiff = p_rate;
}


int8_t PlaneModel::getAilevatorDifferential() const
{
	return m_ailevatorDiff;
}


void PlaneModel::setVTailElevatorMix(int8_t p_rate)
{
	m_vtailEle = p_rate;
}


int8_t PlaneModel::getVTailElevatorMix() const
{
	return m_vtailEle;
}


void PlaneModel::setVTailRudderMix(int8_t p_rate)
{
	m_vtailRud = p_rate;
}


int8_t PlaneModel::getVTailRudderMix() const
{
	return m_vtailRud;
}


void PlaneModel::apply(int16_t p_ail, int16_t p_ele, int16_t p_rud, int16_t p_flp, int16_t p_brk)
{
	switch (m_wing)
	{
	default:
	case WingType_Tailed:
		switch (m_ailerons)
		{
		case AileronCount_4:
			setOutput(Output_AIL4, applyDiff(-p_ail, m_ailDiff));
			setOutput(Output_AIL3, applyDiff( p_ail, m_ailDiff));
			// FALL THROUGH
			
		case AileronCount_2:
			setOutput(Output_AIL2, applyDiff(-p_ail, m_ailDiff));
			setOutput(Output_AIL1, applyDiff( p_ail, m_ailDiff));
			break;
			
		default:
		case AileronCount_1:
			// NOTE: no aileron differential if we have just one aileron servo
			setOutput(Output_AIL1, p_ail);
			break;
		}
		applyTail(p_ail, p_ele, p_rud);
		break;
		
	case WingType_Tailless:
		int16_t ail = mix(p_ail, m_elevonAil);
		int16_t ele = mix(p_ele, m_elevonEle);
		
		switch (m_ailerons)
		{
		case AileronCount_4:
			setOutput(Output_AIL4, applyDiff(-ail, m_ailDiff) + ele);
			setOutput(Output_AIL3, applyDiff( ail, m_ailDiff) + ele);
			// FALL THROUGH
		
		default:		
		case AileronCount_2:
			setOutput(Output_AIL2, applyDiff(-ail, m_ailDiff) + ele);
			setOutput(Output_AIL1, applyDiff( ail, m_ailDiff) + ele);
			break;
		}
		applyRudder(p_rud);
		break;
	}
	
	applyFlaps(p_flp, p_brk);
	applyBrakes(p_brk);
}


void PlaneModel::apply()
{
	apply(getInput(Input_AIL),
	      getInput(Input_ELE),
	      getInput(Input_RUD),
	      getInput(Input_FLP),
	      getInput(Input_BRK));
}


// Private functions

void PlaneModel::applyTail(int16_t p_ail, int16_t p_ele, int16_t p_rud)
{
	switch (m_tail)
	{
	default:
	case TailType_Normal:
		setOutput(Output_ELE1, p_ele);
		setOutput(Output_RUD1, p_rud);
		break;
		
	case TailType_VTail:
		{
			int16_t rud = mix(p_rud, m_vtailRud);
			int16_t ele = mix(p_ele, m_vtailEle);
			setOutput(Output_ELE1, rud + ele); setOutput(Output_RUD2, rud + ele); // V-Tail 1
			setOutput(Output_RUD1, rud - ele); setOutput(Output_ELE2, rud - ele); // V-Tail 2
		}
		break;
		
	case TailType_Ailevator:
		{
			int16_t ail = mix(p_ail, m_ailevator);
			setOutput(Output_ELE1, p_ele + applyDiff( ail, m_ailevatorDiff));
			setOutput(Output_ELE2, p_ele + applyDiff(-ail, m_ailevatorDiff));
			setOutput(Output_RUD1, p_rud);
		}
		break;
	}
}


void PlaneModel::applyRudder(int16_t p_rud)
{
	switch (m_rudder)
	{
	case RudderType_None:
		break;
	
	default:
	case RudderType_Normal:
		setOutput(Output_RUD1, p_rud);
		break;
		
	case RudderType_Winglet:
		setOutput(Output_RUD1, applyDiff(p_rud,  m_wingletDiff));
		setOutput(Output_RUD2, applyDiff(p_rud, -m_wingletDiff));
		break;
	}
}


void PlaneModel::applyFlaps(int16_t p_flp, int16_t p_brk)
{
	switch (m_flaps)
	{
	case FlapCount_4:
		setOutput(Output_FLP4, p_brk);
		setOutput(Output_FLP3, p_brk);
		// FALL THROUGH
	
	case FlapCount_2:
		setOutput(Output_FLP2, p_flp);
		// FALL THROUGH
		
	case FlapCount_1:
		setOutput(Output_FLP1, p_flp);
		// FALL THROUGH
		
	case FlapCount_0:
	default:
		break;
	}
}


void PlaneModel::applyBrakes(int16_t p_brk)
{
	switch (m_brakes)
	{
	case BrakeCount_2:
		setOutput(Output_BRK2, p_brk);
		// FALL THROUGH
		
	case BrakeCount_1:
		setOutput(Output_BRK1, p_brk);
		// FALL THROUGH
	
	case BrakeCount_0:
	default:
		break;
	}
}


int16_t PlaneModel::applyDiff(int16_t p_input, int8_t p_diff) const
{
	if (p_diff == 0 || p_input == 0)
	{
		return p_input;
	}
	if (p_input < 0 && p_diff < 0)
	{
		return p_input;
	}
	if (p_input > 0 && p_diff > 0)
	{
		return p_input;
	}
	return mix(p_input, 100 - (p_diff > 0 ? p_diff : -p_diff));
}


// namespace end
}

