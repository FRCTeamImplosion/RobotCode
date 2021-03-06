#include "RobotDriveControl.h"
#include "SettingsFile.h"
#include "MotorControlHelper.h"

#include <CANTalon.h>
#include <SmartDashboard/SmartDashboard.h>


/*
RobotDriveControl::RobotDriveControl(SettingsFile &settings, JoystickPtr &joystick)
{
	m_joystick = joystick;

	const char *section = "RobotDrive";

	settings.GetSetSetting(section, "ShiftStart", m_initial_shift, 0.65f);
	settings.GetSetSetting(section, "ShiftMin", m_min_shift, 0.3f);
	settings.GetSetSetting(section, "ShiftMax", m_max_shift, 1.0f);
	settings.GetSetSetting(section, "ShiftStep", m_shift_step, 0.35f);
	settings.GetSetSetting(section, "MaxAccel", m_max_accel, 0.025f);
	settings.GetSetSetting(section, "MaxDecel", m_max_decel, 0.05f);

	m_shift_factor = m_initial_shift;

	m_left_speed = 0.0f;
	m_current_left_speed = 0.0f;
	m_right_speed = 0.0f;
	m_current_right_speed = 0.0f;

	m_motors[LEFT_FRONT_MOTOR] = SpeedControllerPtr(MotorControlHelper::ReadSpeedController(settings, section, "LeftFrontMotor"));  // 5
	m_motors[LEFT_BACK_MOTOR] = SpeedControllerPtr(MotorControlHelper::ReadSpeedController(settings, section, "LeftBackMotor"));  // 6
	m_motors[RIGHT_FRONT_MOTOR] = SpeedControllerPtr(MotorControlHelper::ReadSpeedController(settings, section, "RightFrontMotor"));  // 1
	m_motors[RIGHT_BACK_MOTOR] = SpeedControllerPtr(MotorControlHelper::ReadSpeedController(settings, section, "RightBackMotor"));  // 3

	//m_motors[LEFT_FRONT_MOTOR]->SetInverted(false);
	//m_motors[LEFT_BACK_MOTOR]->SetInverted(false);
	//m_motors[RIGHT_FRONT_MOTOR]->SetInverted(true);
	//m_motors[RIGHT_BACK_MOTOR]->SetInverted(true);

	SmartDashboard::PutNumber("Drive/Shift/Start", m_initial_shift);
	SmartDashboard::PutNumber("Drive/Shift/Min", m_min_shift);
	SmartDashboard::PutNumber("Drive/Shift/Max", m_max_shift);
	SmartDashboard::PutNumber("Drive/Shift/Step", m_shift_step);
	SmartDashboard::PutNumber("Drive/Shift/Current", m_shift_factor);
	SmartDashboard::PutNumber("Drive/MaxAccel", m_max_accel);
	SmartDashboard::PutNumber("Drive/MaxDecel", m_max_decel);
}
*/

RobotDriveControl::RobotDriveControl(JoystickPtr &joystick)
{
	m_joystick = joystick;

	m_min_shift = 0.35f;
	m_max_shift = 1.0f;
	m_max_accel = 0.025f;
	m_max_decel = 0.05f;
	m_initial_shift = (m_max_shift - m_min_shift)/2.0f;
	m_shift_step = m_initial_shift;

	m_shift_factor = m_min_shift + m_shift_step;

	m_left_speed = 0.0f;
	m_current_left_speed = 0.0f;
	m_right_speed = 0.0f;
	m_current_right_speed = 0.0f;

	m_motors[LEFT_FRONT_MOTOR] = SpeedControllerPtr(MotorControlHelper::CreateSpeedController("CANTalon", "LeftFrontMotor", 5, true));  // 5
	m_motors[LEFT_BACK_MOTOR] = SpeedControllerPtr(MotorControlHelper::CreateSpeedController("CANTalon", "LeftBackMotor", 6, true));  // 6
	m_motors[RIGHT_FRONT_MOTOR] = SpeedControllerPtr(MotorControlHelper::CreateSpeedController("CANTalon", "RightFrontMotor", 1, false));  // 1
	m_motors[RIGHT_BACK_MOTOR] = SpeedControllerPtr(MotorControlHelper::CreateSpeedController("CANTalon", "RightBackMotor", 1, false));  // 3

	m_left_speed_curve[1.0f] = 1.0f;
	m_left_speed_curve[-1.0f] = 1.0f;

	m_left_speed_curve[-1.0f] = 0.5f;
	m_left_speed_curve[-0.675f] = 0.5f;
	m_left_speed_curve[-0.35f] = 0.7f;
	m_left_speed_curve[0.0f] = 0.7f;
	m_left_speed_curve[0.35f] = 0.7f;
	m_left_speed_curve[0.675f] = 0.6f;
	m_left_speed_curve[1.0f] = 0.6f;

	SmartDashboard::PutNumber("Drive/Shift/Start", m_initial_shift);
	SmartDashboard::PutNumber("Drive/Shift/Min", m_min_shift);
	SmartDashboard::PutNumber("Drive/Shift/Max", m_max_shift);
	SmartDashboard::PutNumber("Drive/Shift/Step", m_shift_step);
	SmartDashboard::PutNumber("Drive/Shift/Current", m_shift_factor);
	SmartDashboard::PutNumber("Drive/MaxAccel", m_max_accel);
	SmartDashboard::PutNumber("Drive/MaxDecel", m_max_decel);
}

RobotDriveControl::~RobotDriveControl()
{
}

void RobotDriveControl::Update(double delta)
{
	// Handle the shift input
	bool shift_changed = false;
	if(m_joystick->IsPressed(XBOX_360_BUTTON_R_BUMPER))
	{
		m_shift_factor += m_shift_step;
		shift_changed = true;
	}
	if(m_joystick->IsPressed(XBOX_360_BUTTON_L_BUMPER))
	{
		m_shift_factor -= m_shift_step;
		shift_changed = true;
	}
	if (shift_changed)
	{
		m_shift_factor = MotorControlHelper::Limit(m_shift_factor, m_min_shift, m_max_shift);
		SmartDashboard::PutNumber("Drive/Shift/Current", m_shift_factor);
	}

	// Handle the stick inputs
	m_left_speed = m_joystick->GetAxis(XBOX_360_AXIS_Y_LEFT);
	m_right_speed = m_joystick->GetAxis(XBOX_360_AXIS_Y_RIGHT);

	int pov = m_joystick->GetPOV();

	SmartDashboard::PutNumber("Drive/POV", pov);
	if (pov >= XBOX_360_POV_DIR_UP)
	{
		switch (pov)
		{
		case XBOX_360_POV_DIR_UP:
			m_left_speed = -1.0f;
			m_right_speed = -1.0f;
			break;
		case XBOX_360_POV_DIR_UP_RIGHT:
			m_left_speed = -1.0f;
			m_right_speed = 0.0f;
			break;
		case XBOX_360_POV_DIR_RIGHT:
			m_left_speed = -1.0f;
			m_right_speed = 1.0f;
			break;
		case XBOX_360_POV_DIR_DOWN_RIGHT:
			m_left_speed = 0.0f;
			m_right_speed = 1.0f;
			break;
		case XBOX_360_POV_DIR_DOWN:
			m_left_speed = 1.0f;
			m_right_speed = 1.0f;
			break;
		case XBOX_360_POV_DIR_DOWN_LEFT:
			m_left_speed = 1.0f;
			m_right_speed = 0.0f;
			break;
		case XBOX_360_POV_DIR_LEFT:
			m_left_speed = 1.0f;
			m_right_speed = -1.0f;
			break;
		case XBOX_360_POV_DIR_UP_LEFT:
			m_left_speed = 0.0f;
			m_right_speed = -1.0f;
			break;
		}
	}


	m_left_speed = MotorControlHelper::Limit(m_left_speed, -1.0f, 1.0f) * m_shift_factor;
	m_right_speed = MotorControlHelper::Limit(m_right_speed, -1.0f, 1.0f) * m_shift_factor;

	// Set the output
	SetMotorSpeeds();
}

void RobotDriveControl::Disable()
{
	m_left_speed = 0.0f;
	m_right_speed = 0.0f;
	m_current_left_speed = 0.0f;
	m_current_right_speed = 0.0f;
	m_shift_factor = m_initial_shift;
	SetMotorSpeeds();
}

void RobotDriveControl::SetMotorSpeeds()
{
	m_current_left_speed = MotorControlHelper::Limit(MotorControlHelper::LimitAccelerationDeceleration(m_current_left_speed, m_left_speed, m_max_accel, m_max_decel));
	m_current_right_speed = MotorControlHelper::Limit(MotorControlHelper::LimitAccelerationDeceleration(m_current_right_speed, m_right_speed, m_max_accel, m_max_decel));

	float left_out = MotorControlHelper::Limit(MotorControlHelper::ScaleSpeed(m_current_left_speed, m_left_speed_curve));
	float right_out = MotorControlHelper::Limit(MotorControlHelper::ScaleSpeed(m_current_right_speed, m_right_speed_curve));

	SmartDashboard::PutNumber("Drive/Speed/Left/Speed", m_left_speed);
	SmartDashboard::PutNumber("Drive/Speed/Left/Current", m_current_left_speed);
	SmartDashboard::PutNumber("Drive/Speed/Left/Output Speed", left_out);
	SmartDashboard::PutNumber("Drive/Speed/Right/Speed", m_right_speed);
	SmartDashboard::PutNumber("Drive/Speed/Right/Current", m_current_right_speed);
	SmartDashboard::PutNumber("Drive/Speed/Right/Output Speed", right_out);

	// Set the motor outputs
	if (m_motors[LEFT_FRONT_MOTOR])
		m_motors[LEFT_FRONT_MOTOR]->Set(left_out);
	if (m_motors[LEFT_BACK_MOTOR])
		m_motors[LEFT_BACK_MOTOR]->Set(left_out);

	if (m_motors[RIGHT_FRONT_MOTOR])
		m_motors[RIGHT_FRONT_MOTOR]->Set(right_out);
	if (m_motors[RIGHT_BACK_MOTOR])
		m_motors[RIGHT_BACK_MOTOR]->Set(right_out);
}
