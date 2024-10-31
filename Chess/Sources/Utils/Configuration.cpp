#include "pch.h"
#include "Configuration.h"

Configuration::Configuration(const std::string & path) :
	m_filePath(path)
{}

Configuration::~Configuration()
{}

void Configuration::save()
{
	lock_guard lock(m_fileLock);

	try {
		m_file.open(m_filePath, std::fstream::out);
		if (!m_file.is_open())
			std::cerr << "Unable to open stream for file: " << m_filePath << std::endl;

		for (auto it : m_properties) {
			if (IncludeComments)
				m_file << commentTag << ' ' << it.second->getComment(VerboseComments) << std::endl;

			m_file << it.second->getName() << delimiter << it.second->valueToString() << std::endl;
			if (IncludeWhiteLine)
				m_file << std::endl;
		}
	}
	catch (const std::fstream::failure& e) {
		std::cerr << e.code().message() << " " << e.what() << std::endl;
	}

	m_file.close();
}

void Configuration::load()
{
	lock_guard lock(m_fileLock);

	try {
		m_file.open(m_filePath, std::fstream::in);
		if (!m_file.is_open())
			std::cerr << "Unable to open stream for file: " << m_filePath << std::endl;

		std::string nextLine;
		while (std::getline(m_file, nextLine)) {
			processLine(nextLine);
		}
	}
	catch (const std::fstream::failure& e) {
		std::cerr << e.code().message() << " " << e.what() << std::endl;
	}

	m_file.close();
}

void Configuration::processLine(const std::string & nextLine)
{
	int nameStart = -1;

	for (int i = 0; i < nextLine.length(); i++) {
		switch (nextLine[i]) {
		case commentTag: //Ignore comment
		{
			if (nameStart < 1)
				return;
			break;
		}
		case delimiter: //Update registered property
		{
			std::string propName = nextLine.substr(nameStart, i - nameStart);
			std::string valueStr = nextLine.substr(i + 1, nextLine.length() - i + 1);

			auto it = m_properties.find(propName);
			if (it != m_properties.end())
				it->second->readProperty(valueStr);

			std::cout << propName << "=" << valueStr << std::endl;
			i = static_cast<int>(nextLine.length());
			break;
		}
		case ' ': //Ignore whitespace
		{
			continue;
			break;
		}
		default: {
			if (nameStart == -1)
				nameStart = i;
			break;
		}
		}
	}
}

void Configuration::registerProperty(ConfigProperty::Ptr configProperty) {
	lock_guard lock(m_fileLock);
	m_properties.emplace(configProperty->getName(), configProperty);
}

ConfigProperty::Ptr Configuration::get(const std::string& key)
{
	lock_guard lock(m_fileLock);
	auto it = m_properties.find(key);
	if (it == m_properties.end())
		return nullptr;
	return it->second;
}

bool Configuration::remove(const std::string & key)
{
	lock_guard lock(m_fileLock);
	auto it = m_properties.find(key);
	if (it == m_properties.end())
		return false;
	m_properties.erase(it);
	return true;
}

void Configuration::clearProperties() {
	lock_guard lock(m_fileLock);
	m_properties.clear();
}


