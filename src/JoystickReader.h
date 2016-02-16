#ifndef JOYSTICKBUTTONS_H
#define JOYSTICKBUTTONS_H

#include "Updatable.h"
#include <Joystick.h>
#include <memory>


enum Xbox360Button
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

	XBOX_360_NUM_BUTTONS
};

enum Xbox360Axis
{
	XBOX_360_AXIS_X_LEFT = 0,
	XBOX_360_AXIS_Y_LEFT,
	XBOX_360_LEFT_TRIGGER,
	XBOX_360_RIGHT_TRIGGER,
	XBOX_360_AXIS_X_RIGHT,
	XBOX_360_AXIS_Y_RIGHT,

	XBOX_360_NUM_AXIS,
};


class JoystickReader : public Updatable
{
public:
	JoystickReader(int stick);

	virtual void Update(double delta);

	float GetAxis(Xbox360Axis axis) const;

	bool IsPressed(Xbox360Button button) const;
	bool IsDown(Xbox360Button button) const;
	bool IsUp(Xbox360Button button) const;
private:
	typedef std::shared_ptr<Joystick> JoystickPtr;
	JoystickPtr m_stick;
	int m_curButtonState;
	int m_lastButtonState;
};

#endif
