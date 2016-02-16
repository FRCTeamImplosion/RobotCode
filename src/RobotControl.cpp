#include "RobotControl.h"
#include "RobotDriveControl.h"

#include <RobotBase.h>
#include <Timer.h>

RobotControl::RobotControl()
{
	for (int cur_stick = 0; cur_stick < NUM_JOYSTICKS; ++cur_stick)
	{
		m_joysticks[cur_stick] = JoystickPtr(new JoystickReader(cur_stick));
		m_updatables.push_back(m_joysticks[cur_stick]);
	}

	m_updatables.push_back(UpdatablePtr(new RobotDriveControl(m_joysticks[DRIVE_STICK])));
}

RobotControl::~RobotControl()
{
}

void RobotControl::RobotInit()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->Initialize();
	}
}

void RobotControl::Disabled()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->Stop();
		ptr->Disable();
	}
}

void RobotControl::Autonomous()
{
	EnableAll();
	StopAll();
}

void RobotControl::OperatorControl()
{
	EnableAll();

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

void RobotControl::EnableAll()
{
	for (UpdatablePtr ptr : m_updatables)
	{
		ptr->Enable();
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
