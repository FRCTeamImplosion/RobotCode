#ifndef ROBOTDRIVECONTROL_H
#define ROBOTDRIVECONTROL_H

#include "Updatable.h"
#include "JoystickReader.h"

#include <SpeedController.h>

#include <memory>

class RobotDriveControl : public Updatable
{
public:
	typedef std::shared_ptr<JoystickReader> JoystickPtr;

	explicit RobotDriveControl(JoystickPtr &joystick);
	~RobotDriveControl();

	virtual void Update(double delta);
	virtual void Disable();
	virtual void Enable();
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
	float Limit(float value) const;
	float Limit(float value, float min, float max) const;
	float LimitAcceleration(float cur_value, float requested_value) const;

	typedef std::shared_ptr<SpeedController> SpeedControllerPtr;
	SpeedControllerPtr m_motors[NUM_DRIVE_MOTORS];

	const float INITIAL_SHIFT = 0.5f;
	const float MAX_SHIFT = 1.0f;
	const float MIN_SHIFT = 0.3f;
	const float SHIFT_STEP = (1.0f - 0.3f) / 2.0f;

	JoystickPtr m_joystick;

	float m_shift_factor;

	float m_left_speed;
	float m_current_left_speed;
	float m_right_speed;
	float m_current_right_speed;
};

#endif
