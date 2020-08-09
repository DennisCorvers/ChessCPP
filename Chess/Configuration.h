#pragma once
#include "ConfigProperty.hpp"
#include <mutex>
#include <fstream>

class Configuration
{
private:
	std::map<std::string, ConfigProperty::Ptr> m_properties;

	std::fstream m_file;
	std::string m_filePath;
	std::mutex m_fileLock;


public:
	Configuration(const std::string& path);
	virtual ~Configuration();

	virtual void save();
	virtual void load();

	void registerProperty(ConfigProperty::Ptr configProperty);
	ConfigProperty::Ptr get(const std::string& key);

	void clearProperties();

	template<typename T>
	std::shared_ptr<T> get(const std::string& key);
};

