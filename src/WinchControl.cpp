#include "WinchControl.h"
#include "JoystickReader.h"
#include "SettingsFile.h"
#include "MotorControlHelper.h"

#include <SmartDashboard/SmartDashboard.h>
#include <CANTalon.h>


WinchControl::WinchControl(SettingsFile &settings, JoystickPtr &joystick)
{
	m_joystick = joystick;

	//const char *section = "Winch";
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
