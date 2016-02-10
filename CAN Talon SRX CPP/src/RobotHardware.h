#ifndef ROBOT_HARDWARE_H
#define ROBOT_HARDWARE_H

#include "Updatable.h"
#include "WPILib.h"
class JoystickButtonsManager;

class RobotHardware : public Updatable{
public:
	RobotHardware();
	~RobotHardware();

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

	void Update(double delta);

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
	JoystickButtonsManager* m_managers[NUM_STICKS];

	RobotDrive* m_drive;

	char* m_motorSettingsFile;
	char* m_joystickSettingsFile;

	const double MAX_SHIFT = 1.0;
	const double MIN_SHIFT = 0;
	const double SHIFT_STEP = .1;
	double m_shiftFactor = .5;

	double m_voltage;

	void LoadMotorSettings();
	void LoadJoystickSettings();

	void SetupMotor(std::string, std::string, void*);
	void SetupControllerMapping(std::string, std::string, void*);

	void Shift();

	void JoystickDrive();
};

#endif
