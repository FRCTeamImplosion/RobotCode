#include "BallControl.h"
#include "JoystickReader.h"
#include "SettingsFile.h"
#include "MotorControlHelper.h"
#include <SmartDashboard/SmartDashboard.h>
#include <Talon.h>


BallControl::BallControl(SettingsFile &settings, JoystickPtr &joystick)
{
	m_joystick = joystick;

	const char *section = "Ball";

	settings.GetSetSetting(section, "MaxInSpeed", m_max_in_speed, 0.4f);
	settings.GetSetSetting(section, "MaxOutSpeed", m_max_out_speed, 0.75f);

	SmartDashboard::PutNumber("Ball/MaxInSpeed", m_max_in_speed);
	SmartDashboard::PutNumber("Ball/MaxOutSpeed", m_max_out_speed);

	m_speed = 0.0f;

	m_motor = SpeedControllerPtr(new Talon(0));
	m_motor->SetInverted(false);
}

BallControl::~BallControl()
{
}

void BallControl::Update(double delta)
{
	// Handle the inputs
	m_speed = 0.0f;
	if (m_joystick->IsDown(XBOX_360_BUTTON_X))
		m_speed = 1.0f;
	if (m_joystick->IsDown(XBOX_360_BUTTON_B))
		m_speed = -1.0f;

	// Set the output
	SetMotorSpeed();
}

void BallControl::Stop()
{
	m_speed = 0.0f;
	SetMotorSpeed();
}

void BallControl::SetMotorSpeed()
{
	float out = MotorControlHelper::Limit(m_speed, -m_max_in_speed, m_max_out_speed);

	SmartDashboard::PutNumber("Ball/Speed", m_speed);
	SmartDashboard::PutNumber("Ball/Out", out);

	// Set the motor outputs
	if (m_motor)
		m_motor->Set(out);
}
