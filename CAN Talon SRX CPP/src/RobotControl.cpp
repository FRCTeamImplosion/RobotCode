#include "WPILib.h"
#include "RobotHardware.h"

/**
 * This sample shows how to use the new CANTalon to just run a motor in a basic
 *   throttle mode, in the same manner as you might control a traditional PWM
 *   controlled motor.
 *
 */
class RobotControl : public SampleRobot {

	// update every 0.01 seconds/10 milliseconds.
  // The talon only receives control packets every 10ms.
	double kUpdatePeriod = 0.010;
	RobotHardware* m_hardware;
public:
 RobotControl()
 {
	 m_hardware = new RobotHardware();
 }

 ~RobotControl(){
	 if(m_hardware)
		 delete m_hardware;
 }

	/**
	 * Runs the motor from the output of a Joystick.
	 */
	void OperatorControl() {
		m_hardware->ReloadIniFile();
		while (IsOperatorControl() && IsEnabled()) {
			// Takes a number from -1.0 (full reverse) to +1.0 (full forwards).
			m_hardware->JoystickDrive();

			Wait(kUpdatePeriod); // Wait a bit so that the loop doesn't lock everything up.
		}
	}
};

START_ROBOT_CLASS(RobotControl);
