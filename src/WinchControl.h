#ifndef WINCHCONTROL_H
#define WINCHCONTROL_H

#include "Updatable.h"
#include "JoystickReader.h"

#include <SpeedController.h>

#include <memory>

class SettingsFile;

class WinchControl : public Updatable
{
public:
	typedef std::shared_ptr<JoystickReader> JoystickPtr;

	//WinchControl(SettingsFile &settings, JoystickPtr &joystick);
	explicit WinchControl(JoystickPtr &joystick);
	~WinchControl();

	virtual void Update(double delta);
	virtual void Stop();

private:
	void SetMotorSpeed();

	typedef std::shared_ptr<SpeedController> SpeedControllerPtr;
	SpeedControllerPtr m_motor;

	JoystickPtr m_joystick;
	float m_speed;
};

#endif
