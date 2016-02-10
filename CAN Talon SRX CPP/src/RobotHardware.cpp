#include "RobotHardware.h"
#include "JoystickButtons.h"
#include "Joystick.h"
#include "IniFile.h"
#include <RobotDrive.h>
#include <iostream>
#include <string>

inline bool IntToBool(int num){ return num != 0; }

/**
 * Constructor for robot hardware
 */
RobotHardware::RobotHardware(){

	for (int curMotor = 0; curMotor < Motors::NUM_DRIVE_MOTORS; ++ curMotor){
		m_motors[curMotor] = 0;
	}
	m_drive = 0;

	for (int curStick = 0; curStick < Joysticks::NUM_STICKS; ++ curStick)
	{
		m_sticks[curStick] = new Joystick(curStick);
		m_managers[curStick] = new JoystickButtonsManager(*(m_sticks[curStick]));
	}
	m_motorSettingsFile = "motors.ini";
	m_joystickSettingsFile = "joysticks.ini";
	m_voltage = ControllerPower::GetInputVoltage();
}

/**
 * Deconstructor for robot hardware
 */
RobotHardware::~RobotHardware()
{

	for (int curMotor = 0; curMotor < Motors::NUM_DRIVE_MOTORS; ++ curMotor){
		if(m_motors[curMotor] != 0)
		{
			delete m_motors[curMotor];
			m_motors[curMotor] = 0;
		}
	}
	if(m_drive != 0)
		delete m_drive;
	m_drive = 0;

	for (int curStick = 0; curStick < Joysticks::NUM_STICKS; ++ curStick)
	{
		delete m_sticks[curStick];
		delete m_managers[curStick];

		m_sticks[curStick] = 0;
		m_managers[curStick] = 0;
	}
}

/**
 * Update all hardware values
 */
void RobotHardware::Update(double delta){
	for(int curManager = 0; curManager < NUM_STICKS; ++curManager)
		m_managers[curManager]->Update();
	Shift();
	JoystickDrive();
	m_voltage = ControllerPower::GetInputVoltage();
}

/**
 * Check the shift input
 */
void RobotHardware::Shift(){
	if(m_managers[DRIVE_STICK]->IsPressed(m_buttonConfigs[ButtonUses::SHIFT_UP]))
	{
		m_shiftFactor += SHIFT_STEP;
		if(m_shiftFactor > MAX_SHIFT)
			m_shiftFactor = MAX_SHIFT;
	}
	else if(m_managers[DRIVE_STICK]->IsPressed(m_buttonConfigs[ButtonUses::SHIFT_UP])){
		m_shiftFactor -= SHIFT_STEP;
		if(m_shiftFactor < MIN_SHIFT)
			m_shiftFactor = MIN_SHIFT;
	}
}

/**
 * Drive using joysticks
 */
void RobotHardware::JoystickDrive(){

	float stickLeftVal = m_sticks[DRIVE_STICK]->GetRawAxis(m_stickConfigs[StickUses::LEFT_STICK]);
	stickLeftVal *= stickLeftVal * m_shiftFactor;

	float stickRightVal = m_sticks[DRIVE_STICK]->GetRawAxis(m_stickConfigs[StickUses::RIGHT_STICK]);
	stickRightVal *= stickRightVal * m_shiftFactor;

	m_drive->TankDrive(stickLeftVal, stickRightVal, true);
}

/**
 * Reload INI file
 */
void RobotHardware::ReloadIniFile()
{
	LoadMotorSettings();
	LoadJoystickSettings();
}

/**
 * Load motor settings
 */
void RobotHardware::LoadMotorSettings(){
	for(int curMotor = 0; curMotor < Motors::NUM_MOTORS; ++curMotor)
	{
		m_motorPorts[curMotor] = curMotor;
		if(m_motors[curMotor] != 0)
			delete m_motors[curMotor];
		m_motors[curMotor] = 0;
	}

	IniFile::PerformFileLineOperation(m_motorSettingsFile, this, &RobotHardware::SetupMotor, 0);

	for (int curMotor = 0; curMotor < Motors::NUM_MOTORS; ++ curMotor){
		m_motors[curMotor] = new CANTalon(m_motorPorts[curMotor]);
	}
	if(m_drive != 0)
		delete m_drive;

	m_drive = new RobotDrive(m_motors[LEFT_FRONT_MOTOR],m_motors[LEFT_BACK_MOTOR],m_motors[RIGHT_FRONT_MOTOR],m_motors[RIGHT_BACK_MOTOR]);

	m_drive->SetInvertedMotor(RobotDrive::kFrontLeftMotor,m_motorsInversion[LEFT_FRONT_MOTOR]);
	m_drive->SetInvertedMotor(RobotDrive::kFrontRightMotor,m_motorsInversion[RIGHT_FRONT_MOTOR]);
	m_drive->SetInvertedMotor(RobotDrive::kRearLeftMotor,m_motorsInversion[LEFT_BACK_MOTOR]);
	m_drive->SetInvertedMotor(RobotDrive::kRearRightMotor,m_motorsInversion[RIGHT_BACK_MOTOR]);
}

/**
 * Load Joystick settings
 */
void RobotHardware::LoadJoystickSettings(){
	for(int curStick = 0; curStick < StickUses::NUM_STICK_USES; ++curStick)
	{
		m_stickConfigs[curStick] = curStick;
	}
	for(int curButton = 0; curButton < ButtonUses::NUM_BUTTON_USES; ++curButton)
	{
		m_buttonConfigs[curButton] = curButton;
	}

	IniFile::PerformFileLineOperation(m_joystickSettingsFile, this, &RobotHardware::SetupControllerMapping, 0);

}

/**
 * Setup all motors from string inputs
 */
void RobotHardware::SetupMotor(std::string curMotor, std::string val, void*)
{
	std::string::size_type sz;
	int num = std::stoi(val, &sz);

	//Drive motor ports
	if(curMotor == "leftFrontMotor")
		m_motorPorts[Motors::LEFT_FRONT_MOTOR] = num;
	if(curMotor == "rightFrontMotor")
		m_motorPorts[Motors::RIGHT_FRONT_MOTOR] = num;
	if(curMotor == "leftBackMotor")
		m_motorPorts[Motors::LEFT_BACK_MOTOR] = num;
	if(curMotor == "rightBackMotor")
		m_motorPorts[Motors::RIGHT_BACK_MOTOR] = num;

	//Drive Motor Inversion Ports
	if(curMotor == "leftFrontMotorInvert")
		m_motorsInversion[Motors::LEFT_FRONT_MOTOR] = IntToBool(num);
	if(curMotor == "rightFrontMotorInvert")
		m_motorsInversion[Motors::RIGHT_FRONT_MOTOR] = IntToBool(num);
	if(curMotor == "leftBackMotorInvert")
		m_motorsInversion[Motors::LEFT_BACK_MOTOR] = IntToBool(num);
	if(curMotor == "rightBackMotorInvert")
		m_motorsInversion[Motors::RIGHT_BACK_MOTOR] = IntToBool(num);
}

/**
 * Setup joystick controller mappings from string values
 */
void RobotHardware::SetupControllerMapping(std::string curStick, std::string val, void*)
{
	int num = 0;

	if(val == "A")
		num = Xbox360Buttons::XBOX_360_BUTTON_A;
	else if(val == "B")
		num = Xbox360Buttons::XBOX_360_BUTTON_B;
	else if(val == "X")
		num = Xbox360Buttons::XBOX_360_BUTTON_X;
	else if(val == "Y")
		num = Xbox360Buttons::XBOX_360_BUTTON_Y;
	else if(val == "L_BUMPER")
		num = Xbox360Buttons::XBOX_360_BUTTON_L_BUMPER;
	else if(val == "R_BUMPER")
		num = Xbox360Buttons::XBOX_360_BUTTON_R_BUMPER;
	else if(val == "BACK")
		num = Xbox360Buttons::XBOX_360_BUTTON_BACK;
	else if(val == "START")
		num = Xbox360Buttons::XBOX_360_BUTTON_START;
	else if(val == "L_STICK_BUTTON")
		num = Xbox360Buttons::XBOX_360_BUTTON_L_STICK;
	else if(val == "R_STICK_BUTTON")
		num = Xbox360Buttons::XBOX_360_BUTTON_R_STICK;
	else if(val == "X_AXIS_L")
		num = Xbox360Axis::XBOX_360_AXIS_X_LEFT;
	else if(val == "Y_AXIS_L")
		num = Xbox360Axis::XBOX_360_AXIS_Y_LEFT;
	else if(val == "X_AXIS_R")
		num = Xbox360Axis::XBOX_360_AXIS_X_RIGHT;
	else if(val == "Y_AXIS_R")
		num = Xbox360Axis::XBOX_360_AXIS_Y_RIGHT;
	else if(val == "TRIGGERS")
		num = Xbox360Axis::XBOX_360_TRIGGERS;

	//Drive motor ports
	if(curStick == "arm_stick")
		m_stickConfigs[StickUses::ARM_STICK] = num;
	if(curStick == "left_stick")
		m_stickConfigs[StickUses::LEFT_STICK] = num;
	if(curStick == "right_stick")
		m_stickConfigs[StickUses::RIGHT_STICK] = num;

	//Drive Motor Inversion Ports
	if(curStick == "shift_up")
		m_buttonConfigs[ButtonUses::SHIFT_UP] = num;
	if(curStick == "shift_down")
		m_buttonConfigs[ButtonUses::SHIFT_DOWN] = num;
	if(curStick == "deploy_pistons")
		m_buttonConfigs[ButtonUses::DEPLOY_PISTIONS] = num;
	if(curStick == "retract_pistons")
		m_buttonConfigs[ButtonUses::RETRACT_PISTONS] = num;
}
