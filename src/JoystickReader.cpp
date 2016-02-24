#include "JoystickReader.h"

#include <Joystick.h>
#include <DriverStation.h>

JoystickReader::JoystickReader(int stick)
{
	m_stick = JoystickPtr(new Joystick(stick));
	m_autonomousButtonState = 0;
	m_curButtonState = 0;
	m_lastButtonState = 0;
	m_povDirection = -1;
	for (int curAxis = 0; curAxis < XBOX_360_NUM_AXIS; curAxis++)
		m_autonomousAxisState[curAxis] = 0.0f;
}

void JoystickReader::Update(double)
{
	m_lastButtonState = m_curButtonState;
	m_curButtonState = 0;

	if (!DriverStation::GetInstance().IsAutonomous())
	{
		for(int curButton = 1; curButton < XBOX_360_NUM_BUTTONS; curButton++)
		{
			if(m_stick->GetRawButton(curButton))
				m_curButtonState |= (1 << curButton);
		}
	}
	else
	{
		m_curButtonState = m_autonomousButtonState;
	}
}

float JoystickReader::GetAxis(Xbox360Axis axis) const
{
	if (!DriverStation::GetInstance().IsAutonomous())
		return m_stick->GetRawAxis(axis);
	return m_autonomousAxisState[axis];
}

int JoystickReader::GetPOV() const
{
	if (!DriverStation::GetInstance().IsAutonomous())
		return m_stick->GetPOV();
	return m_povDirection;
}

void JoystickReader::SetAutonomousButtonState(Xbox360Button button, bool isDown)
{
	if (isDown)
		m_autonomousButtonState |= (1<<button);
	else
		m_autonomousButtonState &= ~(1<<button);
}

void JoystickReader::SetAutonomousAxisValue(Xbox360Axis axis, float value)
{
	m_autonomousAxisState[axis] = value;
}

void JoystickReader::SetAutonomousPOV(Xbox360PovDirection direction)
{
	m_povDirection = direction;
}

bool JoystickReader::IsPressed(Xbox360Button button) const
{
	return ((m_curButtonState & ~m_lastButtonState) & (1 << button)) != 0;
}

bool JoystickReader::IsDown(Xbox360Button button) const
{
	return (m_curButtonState & (1 << button)) != 0;
}

bool JoystickReader::IsUp(Xbox360Button button) const
{
	return (m_curButtonState & (1 << button)) == 0;
}
