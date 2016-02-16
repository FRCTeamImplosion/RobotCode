#ifndef ROBOT_HARDWARE_H
#define ROBOT_HARDWARE_H

#include "Updatable.h"

#include "SpeedController.h"
#include <memory>

class RobotHardware : public Updatable
{
public:
	typedef std::shared_ptr<SpeedController> SpeedControllerPtr;

	RobotHardware();
	~RobotHardware();

	enum MotorId
	{
		//Put drive motors here
		LEFT_FRONT_MOTOR,
		LEFT_BACK_MOTOR,
		RIGHT_FRONT_MOTOR,
		RIGHT_BACK_MOTOR,

		NUM_DRIVE_MOTORS,

		//Put Non-Drive motors here
		ARM_MOTOR = NUM_DRIVE_MOTORS,
		WINCH_MOTOR,
		BALL_MOTOR,

		NUM_MOTORS,
	};

	void Update(double delta);

private:
	std::shared_ptr<SpeedController> m_motors[NUM_MOTORS];
};

#endif
