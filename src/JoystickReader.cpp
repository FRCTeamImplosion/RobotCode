#include "JoystickReader.h"

#include "Joystick.h"

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

	if (!isAutonomous)
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
	if (!isAutonomous)
		return m_stick->GetRawAxis(axis);
	return m_autonomousAxisState[axis];
}

int JoystickReader::GetPOV() const
{
	if (!isAutonomous)
		return m_stick->GetPOV();
	return m_povDirection;
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
