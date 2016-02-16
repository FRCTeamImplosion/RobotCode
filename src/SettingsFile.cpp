#include "SettingsFile.h"
#include <algorithm>
#include <fstream>

#define ENABLE_SAVE 1

#if defined(_MSC_VER) 
	#pragma warning(disable: 4100)
	#pragma warning(disable: 4127)
	#pragma warning(disable: 4996)
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
SettingsFile::SettingsFile()
{
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
SettingsFile::~SettingsFile()
{
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool SettingsFile::SaveSettingsFile(const char *file_name)
{
#if ENABLE_SAVE
	std::ofstream out;
	out.open(file_name);
	if (out.is_open())
	{
		for (SectionMap::iterator section = m_settings.begin(); section != m_settings.end(); section++)
		{
			out << "[" << section->first << "]\n";

			for (KeyMap::iterator key = section->second.begin(); key != section->second.end(); key++)
			{
				out << key->first << "=" << key->second << "\n"; 
			}
			out << "\n";
		}
		out.close();
	}
#endif
	return true;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool SettingsFile::ParseSettingsFile(const char *file_name)
{
	bool result = false;
	std::ifstream in;
	in.open(file_name, std::ios::binary);
	if (in.is_open())
	{
		in.seekg(0, std::ios::end);
		int len = (int)in.tellg();
		in.seekg(0, std::ios::beg);
		if (len > 0)
		{
			char *buffer = new char[len + 1];
			in.read(buffer, len);
			buffer[len] = 0;

			result = ParseSettingsString(buffer);
			delete [] buffer;
		}
		in.close();
	}
	return result;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
const char*	SettingsFile::GetNextLine(const char *cur_line)
{
	if(!cur_line)
		return cur_line;

	while(1)
	{
		// Skip whitespace, carriage returns, line feed, etc.
		while((*cur_line > 0) && (*cur_line <= ' '))
		{
			cur_line++;
		}

		switch(*cur_line)
		{
		case 0:
			return 0;
		case ';':
			// Comment
			cur_line = SkipLine(cur_line);
			break;
		default:
			// Not a comment, not whitespace, not a blank line
			return cur_line;
		}
	}
	// Shouldn't ever get here
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
const char*	SettingsFile::SkipLine(const char *cur_line)
{
	if(!cur_line)
		return cur_line;

	while((*cur_line > 0) && (*cur_line != '\r') && (*cur_line != '\n'))
	{
		cur_line++;
	}
	return cur_line;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
const char*	SettingsFile::FindCharacter(const char *cur_line, char character)
{
	const char
		*find;
	for(find = cur_line + 1; (*find) && (*find != '\r') && (*find != '\n') && (*find != character); find++)
	{
	}
	if(*find == character)
		return find;
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void SettingsFile::TrimString(const char *&start, const char *&end)
{
	for(; (*start) && (start <= end) && (*start <= ' '); start++)
	{
	}
	for(; (*end) && (start <= end) && (*end <= ' '); end--)
	{
	}
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
bool SettingsFile::ParseSettingsString(const char *settings)
{
	if(!settings)
		return false;

	const char *first;
	std::string section;

	// Iterate through all the lines
	for(first = GetNextLine(settings); first && *first; first = GetNextLine(first))
	{
		if((first[0] == '[') && FindCharacter(first + 1, ']'))
		{
			// Found a section head, so create an entry for it
			const char *last = FindCharacter(first + 1, ']');
			if(last - first > 2)
			{
				const char *start = first + 1;
				const char *end = last - 1;
				TrimString(start, end);
				section.assign(start, end - start + 1);
			}
			// Skip what was scanned
			first = last + 1;
		}
		else if((first[0] == ';') || (first[0] == '#'))
		{
			first = SkipLine(first);
		}
		else
		{
			if(section.length() > 0)
			{
				const char *middle = FindCharacter(first + 1, '=');
				if(middle)
				{
					const char *start = first;
					const char *end = middle - 1;
					TrimString(start, end);
					if(end >= start)
					{
						std::string key;
						key.assign(start, end - start + 1);

						start = middle + 1;
						end = SkipLine(middle + 1);
						TrimString(start, end);
						if(end >= start)
						{
							std::string value;
							value.assign(start, end - start + 1);
							m_settings[section][key] = value;
						}
					}
				}
			}

			// Skip the line
			first = SkipLine(first);
		}
	}

	return false;
}

//-------------------------------------------------------------------------
// GetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetting(const char *section, const char *key, std::string &value, const char *def_value) const
{
	SectionMap::const_iterator 
		sect_iter = m_settings.find(section);
	if(sect_iter != m_settings.end())
	{
		KeyMap::const_iterator 
			key_iter = sect_iter->second.find(key);
		if(key_iter != sect_iter->second.end())
		{
			value = key_iter->second;
			return true;
		}
	}
	if(def_value)
		value = def_value;
	return false;
}

//-------------------------------------------------------------------------
// GetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetting(const char *section, const char *key, bool &value, bool def_value) const
{
	std::string 
		setting;
	if(GetSetting(section, key, setting))
	{
		std::transform(setting.begin(), setting.end(), setting.begin(), tolower);
		value = setting == "true" || setting == "1";
		return true;
	}
	value = def_value;
	return false;
}

//-------------------------------------------------------------------------
// GetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetting(const char *section, const char *key, int &value, int def_value) const
{
	std::string 
		setting;
	if(GetSetting(section, key, setting))
	{
		value = atoi(setting.c_str());
		return true;
	}
	value = def_value;
	return false;
}

//-------------------------------------------------------------------------
// GetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetting(const char *section, const char *key, unsigned int &value, unsigned int def_value) const
{
	std::string 
		setting;
	if(GetSetting(section, key, setting))
	{
		value = atoi(setting.c_str());
		return true;
	}
	value = def_value;
	return false;
}

//-------------------------------------------------------------------------
// GetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetting(const char *section, const char *key, float &value, float def_value) const
{
	std::string 
		setting;
	if(GetSetting(section, key, setting))
	{
		value = (float)atof(setting.c_str());
		return true;
	}
	value = def_value;
	return false;
}

//-------------------------------------------------------------------------
// SetSetting
//-------------------------------------------------------------------------
bool SettingsFile::SetSetting(const char *section, const char *key, bool value)
{
	return SetSetting(section, key, value ? "true" : "false");
}

//-------------------------------------------------------------------------
// SetSetting
//-------------------------------------------------------------------------
bool SettingsFile::SetSetting(const char *section, const char *key, int value)
{
	char buffer[64];
	sprintf(buffer, "%d", value);
	return SetSetting(section, key, buffer);
}

//-------------------------------------------------------------------------
// SetSetting
//-------------------------------------------------------------------------
bool SettingsFile::SetSetting(const char *section, const char *key, unsigned int value)
{
	char buffer[64];
	sprintf(buffer, "%d", value);
	return SetSetting(section, key, buffer);
}

//-------------------------------------------------------------------------
// SetSetting
//-------------------------------------------------------------------------
bool SettingsFile::SetSetting(const char *section, const char *key, float value)
{
	char buffer[64];
	sprintf(buffer, "%f", value);
	return SetSetting(section, key, buffer);
}

//-------------------------------------------------------------------------
// SetSetting
//-------------------------------------------------------------------------
bool SettingsFile::SetSetting(const char *section, const char *key, const char *value)
{
	m_settings[section][key] = value;
	return true;
}

//-------------------------------------------------------------------------
// GetSetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetSetting(const char *section, const char *key, bool &value, bool def_value)
{
	if (!GetSetting(section, key, value, def_value))
	{
		SetSetting(section, key, value);
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------
// GetSetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetSetting(const char *section, const char *key, int &value, int def_value)
{
	if (!GetSetting(section, key, value, def_value))
	{
		SetSetting(section, key, value);
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------
// GetSetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetSetting(const char *section, const char *key, unsigned int &value, unsigned int def_value )
{
	if (!GetSetting(section, key, value, def_value))
	{
		SetSetting(section, key, value);
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------
// GetSetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetSetting(const char *section, const char *key, float &value, float def_value)
{
	if (!GetSetting(section, key, value, def_value))
	{
		SetSetting(section, key, value);
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------
// GetSetSetting
//-------------------------------------------------------------------------
bool SettingsFile::GetSetSetting(const char *section, const char *key, std::string &value, const char *def_value)
{
	if (!GetSetting(section, key, value, def_value))
	{
		SetSetting(section, key, value.c_str());
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------
// HasSection
//
// Iterate through the keys in a section
//-------------------------------------------------------------------------
bool  SettingsFile::HasSection(const char* section) const
{
	return m_settings.find(section) != m_settings.end();
}

//-------------------------------------------------------------------------
// GetNextKey
//
// Iterate through the keys in a section
//-------------------------------------------------------------------------
bool SettingsFile::GetFirstKey(const char *section, std::string &key) const
{
	SectionMap::const_iterator 
		sect_iter = m_settings.find(section);
	if(sect_iter != m_settings.end())
	{
		KeyMap::const_iterator 
			key_iter = sect_iter->second.begin();
		if(key_iter != sect_iter->second.end())
		{
			key = key_iter->first;
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------------------
// GetNextKey
//
// Iterate through the keys in a section
//-------------------------------------------------------------------------
bool SettingsFile::GetNextKey(const char *section, std::string &key) const
{
	SectionMap::const_iterator 
		sect_iter = m_settings.find(section);
	if(sect_iter != m_settings.end())
	{
		KeyMap::const_iterator 
			key_iter = sect_iter->second.find(key);
		if(key_iter != sect_iter->second.end())
		{
			// Go to the next key, and if it isn't the end, return the key
			++key_iter;
			if(key_iter != sect_iter->second.end())
			{
				key = key_iter->first;
				return true;
			}
		}
	}
	return false;
}
