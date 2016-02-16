#include "RobotControl.h"
#include "RobotDriveControl.h"

#include <RobotBase.h>
#include <Timer.h>

RobotControl::RobotControl()
{
}

RobotControl::~RobotControl()
{
}

void RobotControl::RobotInit()
{
	m_updatables.clear();

	const char *settings_file_name = "/home/lvuser/RobotSettings.ini";

	m_settings.ParseSettingsString(
			"[RobotDrive]\n"
			"LeftFrontMotor = LeftFrontMotor\n"
			"LeftBackMotor = LeftBackMotor\n"
			"RightFrontMotor = RightFrontMotor\n"
			"RightBackMotor = RightBackMotor\n"
			"LeftScale=-1:-1, 0:0, 1:1\n"
			"ShiftStart=0.65\n"
			"ShiftMin=0.3\n"
			"ShiftMax=1.0\n"
			"ShiftStep=0.35\n"
			"\n"
			);

	for (int cur_stick = 0; cur_stick < NUM_JOYSTICKS; ++cur_stick)
	{
		m_joysticks[cur_stick] = JoystickPtr(new JoystickReader(cur_stick));
		m_updatables.push_back(m_joysticks[cur_stick]);
	}

	m_updatables.push_back(UpdatablePtr(new RobotDriveControl(m_settings, m_joysticks[DRIVE_STICK])));

	m_settings.SaveSettingsFile(settings_file_name);
}

void RobotControl::Disabled()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->Stop();
	}

	RobotInit();	// Reinitialize settings
}

void RobotControl::Autonomous()
{
	StopAll();
}

void RobotControl::OperatorControl()
{
	while (IsOperatorControl() && IsEnabled())
	{
		UpdateAll();
	}
	StopAll();
}

void RobotControl::StopAll()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->Stop();
	}
}

void RobotControl::UpdateAll()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->Update(kUpdatePeriod);
	}
	Wait(kUpdatePeriod); // Wait a bit so that the loop doesn't lock everything up.
}

START_ROBOT_CLASS(RobotControl);
