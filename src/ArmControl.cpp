#include "ArmControl.h"
#include "JoystickReader.h"
#include "SettingsFile.h"
#include "MotorControlHelper.h"

#include <SmartDashboard/SmartDashboard.h>

/*
ArmControl::ArmControl(SettingsFile &settings, JoystickPtr &joystick)
{
	m_joystick = joystick;

	const char *section = "Arm";

	settings.GetSetSetting(section, "MaxDownAccel", m_max_down_acceleration, 0.015f);
	settings.GetSetSetting(section, "MaxDownSpeed", m_max_down_speed, 0.25f);

	m_speed = 0.0f;
	m_current_speed = 0.0f;

	m_left = SolenoidPtr(new Solenoid(1));
	m_right = SolenoidPtr(new Solenoid(2));
	m_left->Set(false);
	m_right->Set(false);

	m_motor = SpeedControllerPtr(MotorControlHelper::ReadSpeedController(settings, "ArmMotor"));

	SmartDashboard::PutNumber("Arm/MaxDownAccel", m_max_down_acceleration);
	SmartDashboard::PutNumber("Arm/MaxDownSpeed", m_max_down_speed);
}
*/


ArmControl::ArmControl(JoystickPtr &joystick)
{
	m_joystick = joystick;

	m_max_down_acceleration = 0.015f;
	m_max_down_speed =  0.25f;

	m_speed = 0.0f;
	m_current_speed = 0.0f;

	m_left = SolenoidPtr(new Solenoid(1));
	m_right = SolenoidPtr(new Solenoid(2));
	m_left->Set(false);
	m_right->Set(false);

	m_motor = SpeedControllerPtr(MotorControlHelper::CreateSpeedController("CANTalon", "ArmMotor", 2, false));

	SmartDashboard::PutNumber("Arm/MaxDownAccel", m_max_down_acceleration);
	SmartDashboard::PutNumber("Arm/MaxDownSpeed", m_max_down_speed);
}

ArmControl::~ArmControl()
{
}

void ArmControl::Update(double delta)
{
	// Handle the stick inputs
	float left_trigger = m_joystick->GetAxis(XBOX_360_LEFT_TRIGGER);
	float right_trigger = m_joystick->GetAxis(XBOX_360_RIGHT_TRIGGER);
	SmartDashboard::PutNumber("Arm/LeftTrigger", left_trigger);
	SmartDashboard::PutNumber("Arm/RightTrigger", right_trigger);

	m_speed = left_trigger - right_trigger;

	// Set the output
	SetMotorSpeed();

	// Handle the buttons for the solenoids
	m_speed = 0.0f;
	if (m_joystick->IsDown(XBOX_360_BUTTON_BACK))
	{
		if (m_left)
			m_left->Set(true);
		if (m_right)
			m_right->Set(true);
	}
	else
	{
		if (m_left)
			m_left->Set(false);
		if (m_right)
			m_right->Set(false);
	}
}

void ArmControl::Stop()
{
	m_speed = 0.0f;
	m_current_speed = 0.0f;
	SetMotorSpeed();
}

void ArmControl::SetMotorSpeed()
{
	//m_current_speed = MotorControlHelper::LimitReverseAcceleration(m_current_speed, m_speed, m_max_down_acceleration);

	float out = MotorControlHelper::Limit(m_current_speed, -m_max_down_speed, 1.0f);

	SmartDashboard::PutNumber("Arm/Speed", m_speed);
	SmartDashboard::PutNumber("Arm/Current", m_current_speed);
	SmartDashboard::PutNumber("Arm/Out", out);

	// Set the motor outputs
	if (m_motor)
		m_motor->Set(out);
}
