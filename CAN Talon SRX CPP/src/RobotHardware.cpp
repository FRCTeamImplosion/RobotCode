#include "RobotHardware.h"

#include "IniFile.h"
#include <RobotDrive.h>
#include <iostream>
#include <string>

inline bool IntToBool(int num){ return num != 0; }


inline bool IsCharNumber(const char& c){
	return (c >= 48 && c <= 57);
}

RobotHardware::RobotHardware(){

	for (int curMotor = 0; curMotor < Motors::NUM_DRIVE_MOTORS; ++ curMotor){
		m_motors[curMotor] = 0;
	}
	m_drive = 0;

	for (int curStick = 0; curStick < Joysticks::NUM_STICKS; ++ curStick)
		m_sticks[curStick] = new Joystick(curStick);

}

void RobotHardware::SetupMotor(std::string curMotor, std::string val, void*)
{
	std::string::size_type sz;
	int num = std::stoi(val, &sz);

	//Drive motor ports
	if(curMotor == "leftFrntMotor")
		m_motorPorts[Motors::LEFT_FRONT_MOTOR] = num;
	if(curMotor == "rghtFrntMotor")
		m_motorPorts[Motors::RIGHT_FRONT_MOTOR] = num;
	if(curMotor == "leftBackMotor")
		m_motorPorts[Motors::LEFT_BACK_MOTOR] = num;
	if(curMotor == "rghtBackMotor")
		m_motorPorts[Motors::RIGHT_BACK_MOTOR] = num;

	//Drive Motor Inversion Ports
	if(curMotor == "leftFrntMInvt")
		m_motorsInversion[Motors::LEFT_FRONT_MOTOR] = IntToBool(num);
	if(curMotor == "rghtFrntMInvt")
		m_motorsInversion[Motors::RIGHT_FRONT_MOTOR] = IntToBool(num);
	if(curMotor == "leftBackMInvt")
		m_motorsInversion[Motors::LEFT_BACK_MOTOR] = IntToBool(num);
	if(curMotor == "rghtBackMInvt")
		m_motorsInversion[Motors::RIGHT_BACK_MOTOR] = IntToBool(num);
}

void RobotHardware::SetupControllerMapping(std::string curStick, std::string val, void*)
{
	std::string::size_type sz;
	int num = std::stoi(val, &sz);

	//Drive motor ports
	if(curStick == "leftFrntMotor")
		m_stickConfigs[StickUses::ARM_STICK] = num;
	if(curStick == "rghtFrntMotor")
		m_stickConfigs[StickUses::LEFT_STICK] = num;
	if(curStick == "leftBackMotor")
		m_stickConfigs[StickUses::RIGHT_STICK] = num;

	//Drive Motor Inversion Ports
	if(curStick == "leftFrntMInvt")
		m_buttonConfigs[ButtonUses::SHIFT_UP] = num;
	if(curStick == "rghtFrntMInvt")
		m_buttonConfigs[ButtonUses::SHIFT_DOWN] = num;
	if(curStick == "leftBackMInvt")
		m_buttonConfigs[ButtonUses::DEPLOY_PISTIONS] = num;
	if(curStick == "rghtBackMInvt")
		m_buttonConfigs[ButtonUses::RETRACT_PISTONS] = num;
}

void RobotHardware::LoadMotorSettings(){
	for(int curMotor = 0; curMotor < Motors::NUM_MOTORS; ++curMotor)
	{
		m_motorPorts[curMotor] = curMotor;
		if(m_motors[curMotor] != 0)
			delete m_motors[curMotor];
		m_motors[curMotor] = 0;
	}

	IniFile::PerformFileLineOperation("motors.ini", this, &RobotHardware::SetupMotor, 0);

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

void RobotHardware::LoadJoystickSettings(){
	for(int curStick = 0; curStick < StickUses::NUM_STICK_USES; ++curStick)
	{
		m_stickConfigs[curStick] = curStick;
	}
	for(int curButton = 0; curButton < ButtonUses::NUM_BUTTON_USES; ++curButton)
	{
		m_buttonConfigs[curButton] = curButton;
	}

	IniFile::PerformFileLineOperation("joysticks.ini", this, &RobotHardware::SetupControllerMapping, 0);

}

void RobotHardware::ReloadIniFile()
{
	LoadMotorSettings();
}

void RobotHardware::JoystickDrive(){
	float stickLeftVal = m_sticks[DRIVE_STICK]->GetRawAxis(m_stickConfigs[StickUses::LEFT_STICK]);
	stickLeftVal *= stickLeftVal;

	float stickRightVal = m_sticks[DRIVE_STICK]->GetRawAxis(m_stickConfigs[StickUses::RIGHT_STICK]);
	stickRightVal *= stickRightVal;

	m_drive->TankDrive(stickLeftVal, stickRightVal, true);
}

