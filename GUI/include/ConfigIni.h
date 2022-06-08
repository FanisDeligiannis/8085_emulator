#pragma once


#define MINI_CASE_SENSITIVE
#include "../../dependencies/mINI/src/mini/ini.h"

namespace ConfigIni {
	extern mINI::INIFile file;
	extern mINI::INIStructure ini;
	
	int GetInt(std::string section, std::string key, int defaultVal);
	std::string GetString(std::string section, std::string key, std::string defaultVal);

	void SetString(std::string section, std::string key, std::string &value);
	void SetInt(std::string section, std::string key, int value);

	void Init();

	void Save();
}