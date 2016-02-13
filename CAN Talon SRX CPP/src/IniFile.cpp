/*
 * IniFile.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: matt
 */

#include <IniFile.h>
#include "RobotHardware.h"
#include <fstream>
#include <sstream>
#include <string>

inline void AddCharToString(std::string * str, size_t pos, char ch)
{
	if(pos < str->length())
		(*str)[pos] = ch;
	else
		str->append(1, ch);
}

IniFile::IniFile() {
	// TODO Auto-generated constructor stub

}

IniFile::~IniFile() {
	// TODO Auto-generated destructor stub
}

void IniFile::PerformFileLineOperation(char filename[], RobotHardware* hardware, FileLineOperation operation, void* param){

	PerformFileLineOperationFake(filename, hardware, operation, param);
	/*
	std::ifstream infile(filename);
	std::string line;

	if(!infile.fail()){
		while (std::getline(infile, line))
		{
			PerformLineOp(line, hardware, operation, param);
		}
	}
	*/
}

void IniFile::PerformFileLineOperationFake(char filename [], RobotHardware* hardware, FileLineOperation operation, void* param){
	std::string files [][25] = {{
			"leftFrontMotor: 1",
			"rightFrontMotor: 2",
			"leftBackMotor: 3",
			"rightBackMotor: 4",
			"leftFrontMotorInvert: 0",
			"rightFrontMotorInvert: 0",
			"leftBackMotorInvert: 0",
			"rightBackMotorInvert: 0",
			"EOF"
	},{
			"arm_stick: TRIGGERS",
			"left_stick: Y_AXIS_L",
			"right_stick: Y_AXIS_R",
			"shift_up: R_BUMPER",
			"shift_down: L_BUMPER",
			"deploy_pistons: A",
			"retract_pistons: B",
			"EOF"
	}};

	std::string file(filename);
	std::string * fileContents = 0;


	if(file == "motors.ini")
	{
		fileContents = files[0];
	}
	else if(file == "joysticks.ini")
	{
		fileContents = files[1];
	}

	if(fileContents == 0)
		return;

	for(int curLine = 0; fileContents[curLine] != "EOF"; ++curLine){
		PerformLineOp(fileContents[curLine], hardware, operation, param);
	}

}

void IniFile::PerformLineOp(std::string line, RobotHardware* hardware, FileLineOperation operation, void* param)
{
	std::string indexName = "";
	//indexName.resize(13);

	bool getName = true;
	std::string val = "";
	for(size_t curCh = 0; curCh < line.length(); ++curCh)
	{
		if(line[curCh]==':')
		{
			getName = false;
			while(curCh + 1 < line.length() && line[curCh+1] == ' ')
				++curCh;
		}
		else if(getName)
		{
			AddCharToString(&indexName, curCh, line[curCh]);
		}
		else{
			AddCharToString(&val, curCh, line[curCh]);
		}
	}
	(hardware->*operation)(indexName, val, param);
}
