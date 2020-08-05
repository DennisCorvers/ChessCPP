#pragma once
#include "ConfigProperty.hpp"
#include <mutex>
#include <fstream>

class Configuration
{
private:
	std::map<std::string, ConfigProperty> m_properties;

	std::fstream m_file;
	std::string m_filePath;
	std::mutex m_fileLock;


public:
	Configuration(const std::string& path);
	virtual ~Configuration();

	virtual void save();
	virtual void load();

	ConfigProperty get(const std::string& key);
};

