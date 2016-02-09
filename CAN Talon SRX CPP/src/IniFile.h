/*
 * IniFile.h
 *
 *  Created on: Feb 8, 2016
 *      Author: matt
 */

#ifndef SRC_INIFILE_H_
#define SRC_INIFILE_H_

#include <string>

class RobotHardware;

class IniFile {
private:
	IniFile();
	virtual ~IniFile();
public:
	typedef void(RobotHardware::* FileLineOperation)(std::string str, std::string, void* param);
	static void PerformFileLineOperation(char [], RobotHardware*, FileLineOperation , void* );
};

#endif /* SRC_INIFILE_H_ */
