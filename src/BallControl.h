#ifndef BALLCONTROL_H
#define BALLCONTROL_H

#include "Updatable.h"
#include "JoystickReader.h"

#include <SpeedController.h>

#include <memory>

class SettingsFile;

class BallControl : public Updatable
{
public:
	typedef std::shared_ptr<JoystickReader> JoystickPtr;

	explicit BallControl(SettingsFile &settings, JoystickPtr &joystick);
	~BallControl();

	virtual void Update(double delta);
	virtual void Stop();

private:
	void SetMotorSpeed();

	typedef std::shared_ptr<SpeedController> SpeedControllerPtr;
	SpeedControllerPtr m_motor;
	JoystickPtr m_joystick;

	float m_max_in_speed;
	float m_max_out_speed;
	float m_speed;
};

#endif
