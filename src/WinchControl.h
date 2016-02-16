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

	explicit WinchControl(SettingsFile &settings, JoystickPtr &joystick);
	~WinchControl();

	virtual void Update(double delta);
	virtual void Stop();

private:
	void SetMotorSpeed();
	float Limit(float value) const;
	float Limit(float value, float min, float max) const;

	typedef std::shared_ptr<SpeedController> SpeedControllerPtr;
	SpeedControllerPtr m_motor;

	JoystickPtr m_joystick;
	float m_speed;
};

#endif
