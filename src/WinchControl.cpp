#include "WinchControl.h"
#include "JoystickReader.h"
#include "SettingsFile.h"
#include "MotorControlHelper.h"

#include <SmartDashboard/SmartDashboard.h>


/*
WinchControl::WinchControl(SettingsFile &settings, JoystickPtr &joystick)
{
	m_joystick = joystick;
	m_speed = 0.0f;
	m_motor = SpeedControllerPtr(MotorControlHelper::ReadSpeedController(settings, "WinchMotor"));
}
*/

WinchControl::WinchControl(JoystickPtr &joystick)
{
	m_joystick = joystick;
	m_speed = 0.0f;
	m_motor = SpeedControllerPtr(MotorControlHelper::CreateSpeedController("CANTalon", "WinchMotor", 4, false));
}

WinchControl::~WinchControl()
{
}

void WinchControl::Update(double delta)
{
	// Handle the stick inputs
	m_speed = 0.0f;
	if (m_joystick->IsDown(XBOX_360_BUTTON_BACK))
		m_speed = -1.0f;
	if (m_joystick->IsDown(XBOX_360_BUTTON_START))
		m_speed = 1.0f;

	// Set the output
	SetMotorSpeed();
}

void WinchControl::Stop()
{
	m_speed = 0.0f;
	SetMotorSpeed();
}

void WinchControl::SetMotorSpeed()
{
	SmartDashboard::PutNumber("Winch/Speed", m_speed);

	float out = MotorControlHelper::Limit(m_speed);

	// Set the motor outputs
	if (m_motor)
		m_motor->Set(out);
}
