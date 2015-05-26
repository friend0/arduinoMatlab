/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** FlycamOne.cpp
** Class to control FlycamOne eco V2 on board camera
** See also: http://www.flycamone.com/
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <wiring.h>
#endif

#include <FlycamOne.h>


namespace rc
{

// Public functions

FlycamOne::FlycamOne(Output p_output)
:
m_camMode(CamMode_Video),
m_sensorMode(SensorMode_Normal),
m_recording(false),
m_command(Command_Idle),
m_startTime(0),
m_duration(0),
m_coolDown(false),
m_value(Value_Low),
m_output(p_output)
{
	
}


void FlycamOne::setOutput(Output p_output)
{
	m_output = p_output;
}


Output FlycamOne::getOutput() const
{
	return m_output;
}


bool FlycamOne::setCamMode(CamMode p_mode)
{
	if (isBusy() || isRecording())
	{
		return false;
	}
	
	if (m_camMode == p_mode)
	{
		return true;
	}
	
	setCommand(getChangeCamCommand(p_mode));
	return true;
}
	

FlycamOne::CamMode FlycamOne::getCamMode() const
{
	return m_camMode;
}

	
bool FlycamOne::setSensorMode(SensorMode p_mode)
{
	if (isBusy() || isRecording())
	{
		return false;
	}
	
	if (m_sensorMode == p_mode)
	{
		return true;
	}
	setCommand(Command_ChangeSensorMode);
	return true;
}
	

FlycamOne::SensorMode FlycamOne::getSensorMode() const
{
	return m_sensorMode;
}


bool FlycamOne::startRecording()
{
	if (isBusy() || isRecording() || m_camMode == CamMode_Photo)
	{
		return false;
	}
	setCommand(Command_StartStop);
	return true;
}


bool FlycamOne::stopRecording()
{
	if (isBusy() || isRecording() == false || m_camMode == CamMode_Photo)
	{
		return false;
	}
	setCommand(Command_StartStop);
	return true;
}


bool FlycamOne::isRecording() const
{
	return m_recording;
}


bool FlycamOne::takePhoto()
{
	if (isBusy() || m_camMode != CamMode_Photo)
	{
		return false;
	}
	setCommand(Command_StartStop);
	return true;
}


bool FlycamOne::isBusy() const
{
	return m_command != Command_Idle;
}


int16_t FlycamOne::update()
{
	if (isBusy())
	{
		if (m_value == Value_Low && m_coolDown == false)
		{
			// just starting
			m_value     = Value_High;
			m_startTime = static_cast<uint16_t>(millis());
		}
		else
		{
			uint16_t delta = static_cast<uint16_t>(millis()) - m_startTime;
			if (delta >= m_duration)
			{
				if (m_coolDown == false)
				{
					m_value = Value_Low;
					switch (m_command)
					{
					case Command_ChangeCamMode:      // FALL THROUGH
					case Command_ChangeCamModeTwice: handleChangeCam();    break;
					case Command_ChangeSensorMode:   handleChangeSensor(); break;
					case Command_StartStop:          handleStartStop();    break;
					default: break;
					}
					m_coolDown = true;
				}
				else if (delta >= m_duration + Timing_CoolDown)
				{
					if (m_command == Command_ChangeCamModeTwice)
					{
						// do it again!
						m_command = Command_ChangeCamMode;
						m_startTime += delta;
					}
					else
					{
						m_command = Command_Idle;
					}
					m_coolDown = false;
				}
			}
		}
	}
	
	if (m_output != Output_None)
	{
		rc::setOutput(m_output, m_value);
	}
	return m_value;
}


// Private functions

void FlycamOne::setCommand(Command p_command)
{
	switch (p_command)
	{
	case Command_ChangeCamMode:      // FALL THROUGH
	case Command_ChangeCamModeTwice: m_duration = Timing_CamMode;    break;
	case Command_ChangeSensorMode:   m_duration = Timing_SensorMode; break;
	case Command_StartStop:          m_duration = Timing_StartStop;  break;
	default:
		m_duration = 0;
		m_command  = Command_Idle;
		return;
	}
	m_command = p_command;
}


FlycamOne::Command FlycamOne::getChangeCamCommand(CamMode p_mode) const
{
	switch (m_camMode)
	{
	default:
	case CamMode_Video:
		if (p_mode == CamMode_Serial)
		{
			return Command_ChangeCamMode;
		}
		else
		{
			return Command_ChangeCamModeTwice; 
		}
		
	case CamMode_Serial:
		if (p_mode == CamMode_Photo)
		{
			return Command_ChangeCamMode;
		}
		else
		{
			return Command_ChangeCamModeTwice;
		}
		
	case CamMode_Photo:
		if (p_mode == CamMode_Video)
		{
			return Command_ChangeCamMode;
		}
		else
		{
			return Command_ChangeCamModeTwice;
		}
	}
}


void FlycamOne::handleChangeCam()
{
	switch (m_camMode)
	{
	default:
	case CamMode_Video:	 m_camMode = CamMode_Serial; break;
	case CamMode_Serial: m_camMode = CamMode_Photo;  break;
	case CamMode_Photo:  m_camMode = CamMode_Video;  break;
	}
}


void FlycamOne::handleChangeSensor()
{
	m_sensorMode = (m_sensorMode == SensorMode_Normal) ? SensorMode_Flipped : SensorMode_Normal;
}


void FlycamOne::handleStartStop()
{
	if (m_camMode != CamMode_Photo)
	{
		m_recording = m_recording == false;
	}
}


// namespace end
}

