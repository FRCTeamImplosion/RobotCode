#ifndef ARMCONTROL_H
#define ARMCONTROL_H

#include "Updatable.h"
#include "JoystickReader.h"

#include <SpeedController.h>
#include <Solenoid.h>

#include <memory>

class SettingsFile;

class ArmControl : public Updatable
{
public:
	typedef std::shared_ptr<JoystickReader> JoystickPtr;

	//explicit ArmControl(SettingsFile &settings, JoystickPtr &joystick);
	explicit ArmControl(JoystickPtr &joystick);
	~ArmControl();

	virtual void RobotInit();
	virtual void AutonomousInit();
	virtual void TeleopInit();

	virtual void Update(double delta);
	virtual void Disable();

private:
	void SetMotorSpeed();

	typedef std::shared_ptr<SpeedController> SpeedControllerPtr;
	typedef std::shared_ptr<Solenoid> SolenoidPtr;
	SpeedControllerPtr m_motor;
	JoystickPtr m_joystick;

	SolenoidPtr m_left;
	SolenoidPtr m_right;

	float m_max_down_acceleration;
	float m_max_down_speed;
	float m_current_speed;
	float m_speed;
};

#endif
