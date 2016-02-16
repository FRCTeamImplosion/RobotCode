#include "WinchControl.h"
#include "JoystickReader.h"
#include "SettingsFile.h"

#include <CANTalon.h>


WinchControl::WinchControl(SettingsFile &settings, JoystickPtr &joystick)
{
	m_joystick = joystick;

	const char *section = "Winch";

	//settings.GetSetSetting(section, "ShiftStart", m_initial_shift, 0.65f);
	//settings.GetSetSetting(section, "ShiftMin", m_min_shift, 0.3f);
	//settings.GetSetSetting(section, "ShiftMax", m_max_shift, 1.0f);
	//settings.GetSetSetting(section, "ShiftStep", m_shift_step, 0.35f);

	m_speed = 0.0f;

	m_motor = SpeedControllerPtr(new CANTalon(4));	// 2 or 4
	m_motor->SetInverted(false);
}

WinchControl::~WinchControl()
{
}

void WinchControl::Update(double delta)
{
	// Handle the stick inputs
	m_speed = m_joystick->GetAxis(XBOX_360_AXIS_Y_LEFT);

	// Set the output
	SetMotorSpeed();
}

void WinchControl::Stop()
{
	m_speed = 0.0f;
	SetMotorSpeed();
}


float WinchControl::Limit(float value) const
{
	return Limit(value, -1.0f, 1.0f);
}

float WinchControl::Limit(float value, float min, float max) const
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}


void WinchControl::SetMotorSpeed()
{
	float out = Limit(m_speed);

	// Set the motor outputs
	if (m_motor)
		m_motor->Set(out);
}
