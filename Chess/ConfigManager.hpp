#pragma once
#include "Configuration.h"

class ConfigManager {
private:
	Configuration m_config;

public:
	unsigned short PortNumber = 61234;
	int EngineDifficulty = 20;

	ConfigManager() :
		m_config("Settings.ini")
	{

	}


private:
	void initConfig() {
		//auto portConfig = new BoolProperty();
	}
};