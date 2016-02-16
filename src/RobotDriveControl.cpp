#include "RobotDriveControl.h"
#include "JoystickReader.h"

#include <CANTalon.h>


RobotDriveControl::RobotDriveControl(JoystickPtr &joystick)
{
	m_joystick = joystick;

	m_shift_factor = MIN_SHIFT + SHIFT_STEP;

	m_left_speed = 0.0f;
	m_current_left_speed = 0.0f;
	m_right_speed = 0.0f;
	m_current_right_speed = 0.0f;

	m_motors[LEFT_FRONT_MOTOR] = SpeedControllerPtr(new CANTalon(5));
	m_motors[LEFT_BACK_MOTOR] = SpeedControllerPtr(new CANTalon(6));
	m_motors[RIGHT_FRONT_MOTOR] = SpeedControllerPtr(new CANTalon(1));
	m_motors[RIGHT_BACK_MOTOR] = SpeedControllerPtr(new CANTalon(3));

	m_motors[LEFT_FRONT_MOTOR]->SetInverted(false);
	m_motors[LEFT_BACK_MOTOR]->SetInverted(false);
	m_motors[RIGHT_FRONT_MOTOR]->SetInverted(true);
	m_motors[RIGHT_BACK_MOTOR]->SetInverted(true);
}

RobotDriveControl::~RobotDriveControl()
{
}

void RobotDriveControl::Update(double delta)
{
	// Handle the shift input
	if(m_joystick->IsPressed(XBOX_360_BUTTON_R_BUMPER))
		m_shift_factor += SHIFT_STEP;
	if(m_joystick->IsPressed(XBOX_360_BUTTON_L_BUMPER))
		m_shift_factor -= SHIFT_STEP;
	m_shift_factor = Limit(m_shift_factor, MIN_SHIFT, MAX_SHIFT);

	// Handle the stick inputs
	m_left_speed = m_joystick->GetAxis(XBOX_360_AXIS_Y_LEFT) * m_shift_factor;
	m_right_speed = m_joystick->GetAxis(XBOX_360_AXIS_Y_RIGHT) * m_shift_factor;

	// Set the output
	SetMotorSpeeds();
}

void RobotDriveControl::Disable()
{
	Stop();
}

void RobotDriveControl::Enable()
{
	Stop();
	m_shift_factor = INITIAL_SHIFT;
}

void RobotDriveControl::Stop()
{
	m_left_speed = 0.0f;
	m_right_speed = 0.0f;
	m_current_left_speed = 0.0f;
	m_current_right_speed = 0.0f;
	SetMotorSpeeds();
}


float RobotDriveControl::Limit(float value) const
{
	return Limit(value, -1.0f, 1.0f);
}

float RobotDriveControl::Limit(float value, float min, float max) const
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

float RobotDriveControl::ScaleLeftSpeed(float value) const
{
	return value;
}

float RobotDriveControl::ScaleRightSpeed(float value) const
{
	return value;
}

float RobotDriveControl::LimitAcceleration(float cur_value, float requested_value) const
{
	return requested_value;
}

void RobotDriveControl::SetMotorSpeeds()
{
	m_current_left_speed = LimitAcceleration(m_current_left_speed, m_left_speed);
	m_current_right_speed = LimitAcceleration(m_current_right_speed, m_right_speed);

	float left_out = Limit(ScaleLeftSpeed(m_current_left_speed));
	float right_out = Limit(ScaleRightSpeed(m_current_right_speed));

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
