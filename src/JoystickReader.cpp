#include "JoystickReader.h"

#include "Joystick.h"

JoystickReader::JoystickReader(int stick)
{
	m_stick = JoystickPtr(new Joystick(stick));
	m_curButtonState = 0;
	m_lastButtonState = 0;
}

void JoystickReader::Update(double)
{
	m_lastButtonState = m_curButtonState;
	m_curButtonState = 0;

	for(int curButton = 1; curButton < XBOX_360_NUM_BUTTONS; curButton++)
	{
		if(m_stick->GetRawButton(curButton))
			m_curButtonState |= (1 << curButton);
	}
}

float JoystickReader::GetAxis(Xbox360Axis axis) const
{
	return m_stick->GetRawAxis(axis);
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