#include "RobotHardware.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

inline bool IntToBool(int num){ return num != 0; }

inline void AddCharToString(std::string * str, size_t pos, char ch)
{
	if(pos < str->length())
		(*str)[pos] = ch;
	else
		str->append(1, ch);
}

inline bool IsCharNumber(const char& c){
	return (c >= 48 && c <= 57);
}

RobotHardware::RobotHardware(){

	for (int curMotor = 0; curMotor < Motors::NUM_DRIVE_MOTORS; ++ curMotor){
		if(m_motors[curMotor] != 0)
			delete m_motors[curMotor];
	}
}

void RobotHardware::LoadMotorSettings(){
	for(int curMotor = 0; curMotor < Motors::NUM_MOTORS; ++curMotor)
	{
		m_motorPorts[curMotor] = curMotor;
		m_motors[curMotor] = 0;
	}

	std::ifstream infile("motors.ini");
	std::string line;

	if(!infile.fail()){
		while (std::getline(infile, line))
		{
			std::string curMotor = "";
			curMotor.resize(13);

			bool getName = true;
			int num = 0;
			for(size_t curCh = 0; curCh < line.length(); ++curCh)
			{
				if(line[curCh]==':')
				{
					getName = false;
				}
				else if(getName)
				{
					AddCharToString(&curMotor, curCh, line[curCh]);
				}
				else{
					if(IsCharNumber(line[curCh]))
					{
						num = (num * 10) + (line[curCh] - 48);
					}
				}
			}

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
	}

	for (int curMotor = 0; curMotor < Motors::NUM_MOTORS; ++ curMotor){
		m_motors[curMotor] = new CANTalon(m_motorPorts[curMotor]);
	}
}

void RobotHardware::ReloadIniFile()
{
	LoadMotorSettings();
}

