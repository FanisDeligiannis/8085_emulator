#include "ConfigIni.h"

//Pretty straight forward, using the mINI header lib.

namespace ConfigIni
{
	mINI::INIFile file("config.ini");
	mINI::INIStructure ini;


	void Init()
	{
		bool c = file.read(ini);
	}

	void Save()
	{
		file.generate(ini);
	}
	
	int GetInt(std::string section, std::string key, int defaultVal)
	{
		if (ini.has(section) && ini[section].has(key))
		{
			std::string& configVal = ini[section][key];
			
			int ret; 
			try
			{
				ret = std::stoi(configVal);
			}
			catch (...)
			{
				ret = defaultVal;
			}
			return ret;
		}
		else
		{
			return defaultVal;
		}
	}
	
	std::string GetString(std::string section, std::string key, std::string defaultVal)
	{
		if (ini.has(section) && ini[section].has(key))
		{
			std::string& configVal = ini[section][key];
			return configVal;
		}
		else
		{
			return defaultVal;
		}
	}
	
	void SetString(std::string section, std::string key, std::string value)
	{
		ini[section][key] = value;
		Save();
	}

	void SetInt(std::string section, std::string key, int value)
	{
		ini[section][key] = std::to_string(value);
		Save();
	}
}


