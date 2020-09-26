#pragma once
#include "ConfigProperty.hpp"
#include <mutex>
#include <fstream>

class Configuration
{
private:
	static const char delimiter = '=';
	static const char commentTag = '#';
	
	std::map<std::string, ConfigProperty::Ptr> m_properties;

	std::fstream m_file;
	std::string m_filePath;
	std::mutex m_fileLock;
	using lock_guard = const std::lock_guard<std::mutex>;

	void processLine(const std::string& nextLine);

public:
	bool IncludeComments = true;
	bool IncludeWhiteLine = true;
	bool VerboseComments = true;

	Configuration(const std::string& path);
	virtual ~Configuration();

	virtual void save();
	virtual void load();

	void registerProperty(ConfigProperty::Ptr configProperty);
	ConfigProperty::Ptr get(const std::string& key);
	bool remove(const std::string& key);

	void clearProperties();

	template<typename T>
	std::shared_ptr<T> getType(const std::string& key)
	{
		static_assert(std::is_base_of<ConfigProperty, T>::value, "Provided type is not a ConfigProperty.");

		auto it = m_properties.find(key);
		if (it == m_properties.end())
			throw new std::exception(("Property with key " + key + " does not exist.").c_str());

		auto baseProp = std::dynamic_pointer_cast<T>(it->second);

		return baseProp;
	}

	template<typename T, typename... Types>
	std::shared_ptr<T> registerProperty(Types&&... args) {
		static_assert(std::is_base_of<ConfigProperty, T>::value, "Provided type is not a ConfigProperty.");

		std::shared_ptr<T> newProperty = std::make_shared<T>(std::forward<Types>(args)...);

		lock_guard lock(m_fileLock);
		m_properties.emplace(newProperty->getName(), newProperty);

		return newProperty;
	}
};


