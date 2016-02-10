#include "WPILib.h"
#include "RobotHardware.h"

class RobotControl : public SampleRobot {

	// update every 0.01 seconds/10 milliseconds.
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

	void OperatorControl() {
		m_hardware->ReloadIniFile();
		while (IsOperatorControl() && IsEnabled()) {
			m_hardware->Update(kUpdatePeriod);

			Wait(kUpdatePeriod); // Wait a bit so that the loop doesn't lock everything up.
		}
	}
};

START_ROBOT_CLASS(RobotControl);
