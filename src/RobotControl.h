#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H

#include "Updatable.h"
#include "JoystickReader.h"
#include "SettingsFile.h"

#include <SampleRobot.h>
#include <Compressor.h>
#include <memory>

class RobotControl : public SampleRobot
{
public:
	RobotControl();
	virtual ~RobotControl();
	virtual void RobotInit();
	virtual void Disabled();
	virtual void Autonomous();
	virtual void OperatorControl();
private:
	void DisableAll();
	void UpdateAll();

	enum Joysticks
	{
		DRIVE_STICK,
		ARM_STICK,
		NUM_JOYSTICKS
	};

	// update every 0.01 seconds/10 milliseconds.
	double kUpdatePeriod = 0.01;

	typedef std::shared_ptr<Updatable> UpdatablePtr;
	std::vector<UpdatablePtr>	m_updatables;

	typedef std::shared_ptr<JoystickReader> JoystickPtr;
	JoystickPtr m_joysticks[NUM_JOYSTICKS];

	typedef std::shared_ptr<Compressor> CompressorPtr;
	CompressorPtr m_compressor;

	//SettingsFile m_settings;
};

#endif
