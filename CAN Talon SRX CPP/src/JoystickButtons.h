#ifndef JOYSTICKBUTTONS_H
#define JOYSTICKBUTTONS_H

enum Xbox360Buttons
{
	XBOX_360_BUTTON_A = 1,
	XBOX_360_BUTTON_B,
	XBOX_360_BUTTON_X,
	XBOX_360_BUTTON_Y,
	XBOX_360_BUTTON_L_BUMPER,
	XBOX_360_BUTTON_R_BUMPER,
	XBOX_360_BUTTON_BACK,
	XBOX_360_BUTTON_START,
	XBOX_360_BUTTON_L_STICK,
	XBOX_360_BUTTON_R_STICK,

	XBOX_360_NUM_BUTTONS,
};

enum Xbox360Axis
{
	XBOX_360_AXIS_X_LEFT = 1,
	XBOX_360_AXIS_Y_LEFT,
	XBOX_360_TRIGGERS,
	XBOX_360_AXIS_X_RIGHT,
	XBOX_360_AXIS_Y_RIGHT,

	XBOX_360_NUM_AXIS,
};

class Joystick;

class JoystickButtonsManager{
public:
	JoystickButtonsManager(Joystick& stick);

	void Update();

	bool IsPressed(int button);
	bool IsDown(int button);
	bool IsUp(int button);
private:
	Joystick* m_stick;
	int m_curButtonState;
	int m_lastButtonState;
};

#endif
