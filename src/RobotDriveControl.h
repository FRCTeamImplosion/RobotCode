#ifndef ROBOTDRIVECONTROL_H
#define ROBOTDRIVECONTROL_H

#include "Updatable.h"
#include "JoystickReader.h"

#include <SpeedController.h>

#include <memory>
#include <map>

class SettingsFile;

class RobotDriveControl : public Updatable
{
public:
	typedef std::shared_ptr<JoystickReader> JoystickPtr;

	explicit RobotDriveControl(SettingsFile &settings, JoystickPtr &joystick);
	~RobotDriveControl();

	virtual void Update(double delta);
	virtual void Stop();

private:
	enum MotorId
	{
		//Put drive motors here
		LEFT_FRONT_MOTOR,
		LEFT_BACK_MOTOR,
		RIGHT_FRONT_MOTOR,
		RIGHT_BACK_MOTOR,

		NUM_DRIVE_MOTORS,
	};

	float ScaleLeftSpeed(float value) const;
	float ScaleRightSpeed(float value) const;
	void SetMotorSpeeds();

	typedef std::shared_ptr<SpeedController> SpeedControllerPtr;
	SpeedControllerPtr m_motors[NUM_DRIVE_MOTORS];

	float m_max_accel;
	float m_max_decel;

	float m_initial_shift;
	float m_max_shift;
	float m_min_shift;
	float m_shift_step;

	JoystickPtr m_joystick;

	float m_shift_factor;

	float m_left_speed;
	float m_current_left_speed;
	float m_right_speed;
	float m_current_right_speed;

	std::map<float, float> 	m_left_speed_curve;
	std::map<float, float>  m_right_speed_curve;
};

#endif
