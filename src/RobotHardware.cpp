#include "RobotHardware.h"
#include "CANTalon.h"
#include "Talon.h"

//#include "JoystickButtons.h"
//#include "Joystick.h"
//#include "IniFile.h"
//#include <RobotDrive.h>

/**
 * Constructor for robot hardware
 */
RobotHardware::RobotHardware()
{

/*
	m_motors[ARM_MOTOR] = new SpeedControllerPtr(CANTalon(5));
	m_motors[ARM_MOTOR]->SetInverted(false);

	m_motors[WINCH_MOTOR] = SpeedControllerPtr(new CANTalon(6));
	m_motors[WINCH_MOTOR]->SetInverted(false);

	m_motors[BALL_MOTOR] = SpeedControllerPtr(new Talon(0));
	m_motors[BALL_MOTOR]->SetInverted(false);
*/
}

/**
 * Deconstructor for robot hardware
 */
RobotHardware::~RobotHardware()
{
}

/**
 * Update all hardware values
 */
void RobotHardware::Update(double delta)
{
}
