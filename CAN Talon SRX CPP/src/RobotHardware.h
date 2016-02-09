#ifndef ROBOT_HARDWARE_H
#define ROBOT_HARDWARE_H

#include "WPILib.h"

class RobotHardware{
public:
	RobotHardware();

	void ReloadIniFile();

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

	enum Joysticks{
		DRIVE_STICK,

		NUM_STICKS
	};

	void JoystickDrive();

private:

	enum StickUses{
		LEFT_STICK,
		RIGHT_STICK,
		ARM_STICK,

		NUM_STICK_USES
	};

	enum ButtonUses{
		SHIFT_UP,
		SHIFT_DOWN,
		DEPLOY_PISTIONS,
		RETRACT_PISTONS,

		NUM_BUTTON_USES
	};

	int m_motorPorts[Motors::NUM_MOTORS];
	SpeedController* m_motors[Motors::NUM_MOTORS];
	bool m_motorsInversion[Motors::NUM_MOTORS];

	int m_stickConfigs[NUM_STICK_USES];
	int m_buttonConfigs[NUM_BUTTON_USES];

	Joystick* m_sticks[NUM_STICKS];

	RobotDrive* m_drive;

	void LoadMotorSettings();
	void LoadJoystickSettings();

	void SetupMotor(std::string, std::string, void*);
	void SetupControllerMapping(std::string, std::string, void*);
};

#endif
