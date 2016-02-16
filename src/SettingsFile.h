#ifndef SETTINGSFILE_H
#define SETTINGSFILE_H

#include <string>
#include <map>

class SettingsFile
{
public:
				SettingsFile();
				~SettingsFile();

	bool        SaveSettingsFile(const char *file_name);
	bool        ParseSettingsFile(const char *file_name);
	bool		ParseSettingsString(const char *settings);
	bool		HasSection(const char *section) const;

	bool        GetSetting(const char *section, const char *key, bool &value, bool def_value = false) const;
	bool        GetSetting(const char *section, const char *key, int &value, int def_value = 0) const;
	bool        GetSetting(const char *section, const char *key, unsigned int &value, unsigned int def_value = 0) const;
	bool        GetSetting(const char *section, const char *key, float &value, float def_value = 0) const;
	bool        GetSetting(const char *section, const char *key, std::string &value, const char *def_value = 0) const;

	bool        SetSetting(const char *section, const char *key, bool value);
	bool        SetSetting(const char *section, const char *key, int value);
	bool        SetSetting(const char *section, const char *key, unsigned int value);
	bool        SetSetting(const char *section, const char *key, float value);
	bool        SetSetting(const char *section, const char *key, const char *value);

	bool        GetSetSetting(const char *section, const char *key, bool &value, bool def_value = false);
	bool        GetSetSetting(const char *section, const char *key, int &value, int def_value = 0);
	bool        GetSetSetting(const char *section, const char *key, unsigned int &value, unsigned int def_value = 0);
	bool        GetSetSetting(const char *section, const char *key, float &value, float def_value = 0);
	bool        GetSetSetting(const char *section, const char *key, std::string &value, const char *def_value = 0);


	bool		GetFirstKey(const char *section, std::string &value) const;
	bool		GetNextKey(const char *section, std::string &value) const;

private:
	typedef std::map<std::string, std::string>   KeyMap;
	typedef std::map<std::string, KeyMap>		 SectionMap;

	static	const char*	GetNextLine(const char *cur_line);
	static	const char*	SkipLine(const char *cur_line);
	static	const char*	FindCharacter(const char *cur_line, char character);
	static	void		TrimString(const char *&start, const char *&end);

    SectionMap   m_settings;
};

#endif

