#ifndef ARMCONTROL_H
#define ARMCONTROL_H

#include "Updatable.h"
#include "JoystickReader.h"

#include <SpeedController.h>

#include <memory>

class SettingsFile;

class ArmControl : public Updatable
{
public:
	typedef std::shared_ptr<JoystickReader> JoystickPtr;

	explicit ArmControl(SettingsFile &settings, JoystickPtr &joystick);
	~ArmControl();

	virtual void Update(double delta);
	virtual void Stop();

private:
	float Limit(float value) const;
	float Limit(float value, float min, float max) const;
	float LimitAcceleration(float cur_value, float requested_value) const;
	void SetMotorSpeed();

	typedef std::shared_ptr<SpeedController> SpeedControllerPtr;
	SpeedControllerPtr m_motor;
	JoystickPtr m_joystick;
	float m_current_speed;
	float m_speed;
};

#endif
