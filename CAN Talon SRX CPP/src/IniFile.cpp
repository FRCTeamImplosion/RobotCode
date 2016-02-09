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

	std::ifstream infile(filename);
	std::string line;

	if(!infile.fail()){
		while (std::getline(infile, line))
		{
			std::string indexName = "";
			indexName.resize(13);

			bool getName = true;
			std::string val = "";
			for(size_t curCh = 0; curCh < line.length(); ++curCh)
			{
				if(line[curCh]==':')
				{
					getName = false;
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
	}
}
