#include "RobotControl.h"
#include "RobotDriveControl.h"
#include "ArmControl.h"
#include "BallControl.h"
#include "WinchControl.h"

#include <DriverStation.h>
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
	for (int cur_stick = 0; cur_stick < NUM_JOYSTICKS; ++cur_stick)
	{
		m_joysticks[cur_stick] = 0;
	}

	m_compressor = CompressorPtr(new Compressor());
	m_compressor->Start();

	//const char *settings_file_name = "/home/lvuser/RobotSettings.ini";

	//m_settings.ParseSettingsFile(settings_file_name);

	for (int cur_stick = 0; cur_stick < NUM_JOYSTICKS; ++cur_stick)
	{
		m_joysticks[cur_stick] = JoystickPtr(new JoystickReader(cur_stick));
		m_updatables.push_back(m_joysticks[cur_stick]);
	}

	m_updatables.push_back(UpdatablePtr(new RobotDriveControl( m_joysticks[DRIVE_STICK])));
	m_updatables.push_back(UpdatablePtr(new ArmControl( m_joysticks[ARM_STICK])));
	m_updatables.push_back(UpdatablePtr(new WinchControl( m_joysticks[ARM_STICK])));
	m_updatables.push_back(UpdatablePtr(new BallControl( m_joysticks[ARM_STICK])));

	//m_settings.SaveSettingsFile(settings_file_name);

	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->RobotInit();
	}

	DriverStation::ReportWarning("RobotInit called");
}

void RobotControl::Disabled()
{
	DriverStation::ReportWarning("Disabled called");
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->Disable();
	}
}

void RobotControl::Autonomous()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->AutonomousInit();
	}

	DriverStation::ReportWarning("Autonomous called");

	for (int shift = 0; shift < 4; shift++)
	{
		m_joysticks[DRIVE_STICK]->SetAutonomousButtonState(XBOX_360_BUTTON_L_BUMPER, true);
		AutonomousWait(0.03);
		m_joysticks[DRIVE_STICK]->SetAutonomousButtonState(XBOX_360_BUTTON_L_BUMPER, false);
		AutonomousWait(0.03);
	}
	/*
	for (int shift = 0; shift < 1; shift++)
	{
		m_joysticks[DRIVE_STICK]->SetAutonomousButtonState(XBOX_360_BUTTON_R_BUMPER, true);
		AutonomousWait(0.03);
		m_joysticks[DRIVE_STICK]->SetAutonomousButtonState(XBOX_360_BUTTON_R_BUMPER, false);
		AutonomousWait(0.03);
	}
	*/
	m_joysticks[DRIVE_STICK]->SetAutonomousPOV(XBOX_360_POV_DIR_UP);
	AutonomousWait(3.5);


	m_joysticks[DRIVE_STICK]->SetAutonomousPOV(XBOX_360_POV_DIR_NONE);
	for (int shift = 0; shift < 1; shift++)
	{
		m_joysticks[DRIVE_STICK]->SetAutonomousButtonState(XBOX_360_BUTTON_R_BUMPER, true);
		AutonomousWait(0.03);
		m_joysticks[DRIVE_STICK]->SetAutonomousButtonState(XBOX_360_BUTTON_R_BUMPER, false);
		AutonomousWait(0.03);
	}

	DisableAll();
}

void RobotControl::AutonomousWait(double seconds)
{
	Timer autonomousTimer;
	autonomousTimer.Start();
	while (IsAutonomous() && IsEnabled() && autonomousTimer.Get() < seconds)
	{
		UpdateAll();
	}
}

void RobotControl::OperatorControl()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->TeleopInit();
	}

	DriverStation::ReportWarning("OperatorControl called");
	while (IsOperatorControl() && IsEnabled())
	{
		UpdateAll();
	}
	DisableAll();
}

void RobotControl::DisableAll()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->Disable();
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
