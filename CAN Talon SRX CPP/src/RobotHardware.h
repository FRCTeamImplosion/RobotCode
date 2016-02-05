#ifndef ROBOT_HARDWARE_H
#define ROBOT_HARDWARE_H

#include "WPILib.h"

class RobotHardware{
public:
	RobotHardware();

	void ReloadIniFile();
private:

	enum Motors{
		//Put drive motors here
		LEFT_FRONT_MOTOR,
		LEFT_BACK_MOTOR,
		RIGHT_FRONT_MOTOR,
		RIGHT_BACK_MOTOR,

		NUM_DRIVE_MOTORS,

		//Put Non-Drive motors here
		ARM_MOTOR = NUM_DRIVE_MOTORS,

		NUM_MOTORS,
	};

	int m_motorPorts[Motors::NUM_MOTORS];
	SpeedController* m_motors[Motors::NUM_MOTORS];
	bool m_motorsInversion[Motors::NUM_MOTORS];

	void LoadMotorSettings();
};

#endif
