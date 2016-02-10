#include "JoystickButtons.h"
#include "WPILib.h"

JoystickButtonsManager::JoystickButtonsManager(Joystick& stick)
{
	m_stick = &stick;
	m_curButtonState = 0;
	m_lastButtonState = 0;
}

void JoystickButtonsManager::Update(){
	m_lastButtonState = m_curButtonState;
	m_curButtonState = 0;
	for(int curButton = 0; curButton < XBOX_360_NUM_BUTTONS; curButton++)
	{
		if(m_stick->GetRawButton(curButton))
			m_curButtonState |= 1 << curButton;
	}
}

bool JoystickButtonsManager::IsPressed(int button){
	return ((m_curButtonState >> button) | 1) ^ ((m_lastButtonState >> button) | 1);
}

bool JoystickButtonsManager::IsDown(int button){
	return ((m_curButtonState >> button) | 1);
}

bool JoystickButtonsManager::IsUp(int button){
	return !((m_curButtonState >> button) | 1);
}
