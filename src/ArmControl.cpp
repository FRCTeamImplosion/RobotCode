#include "ArmControl.h"
#include "JoystickReader.h"
#include "SettingsFile.h"

#include <CANTalon.h>


ArmControl::ArmControl(SettingsFile &settings, JoystickPtr &joystick)
{
	m_joystick = joystick;

	const char *section = "Arm";

	//settings.GetSetSetting(section, "ShiftStart", m_initial_shift, 0.65f);
	//settings.GetSetSetting(section, "ShiftMin", m_min_shift, 0.3f);
	//settings.GetSetSetting(section, "ShiftMax", m_max_shift, 1.0f);
	//settings.GetSetSetting(section, "ShiftStep", m_shift_step, 0.35f);

	m_speed = 0.0f;
	m_current_speed = 0.0f;

	m_motor = SpeedControllerPtr(new CANTalon(2));	// 2 or 4 is left
	m_motor->SetInverted(false);
}

ArmControl::~ArmControl()
{
}

void ArmControl::Update(double delta)
{
	// Handle the stick inputs
	m_speed = m_joystick->GetAxis(XBOX_360_LEFT_TRIGGER) - m_joystick->GetAxis(XBOX_360_RIGHT_TRIGGER);

	// Set the output
	SetMotorSpeed();
}

void ArmControl::Stop()
{
	m_speed = 0.0f;
	m_current_speed = 0.0f;
	SetMotorSpeed();
}


float ArmControl::Limit(float value) const
{
	return Limit(value, -1.0f, 1.0f);
}

float ArmControl::Limit(float value, float min, float max) const
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

float ArmControl::LimitAcceleration(float cur_value, float requested_value) const
{
	return requested_value;
}

void ArmControl::SetMotorSpeed()
{
	m_current_speed = LimitAcceleration(m_current_speed, m_speed);

	float out = Limit(m_current_speed);

	// Set the motor outputs
	if (m_motor)
		m_motor->Set(out);
}
