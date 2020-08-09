#include "pch.h"
#include "Configuration.h"

Configuration::Configuration(const std::string & path) :
	m_filePath(path)
{}

Configuration::~Configuration()
{}

void Configuration::save()
{
	try {
		m_file.open(m_filePath, std::fstream::out);


	}
	catch (const std::fstream::failure& e) {
		std::cerr << e.code().message() << " " << e.what() << std::endl;
	}

	m_file.close();
}

void Configuration::load()
{

}

void Configuration::clearProperties() {
	m_properties.clear();
}


