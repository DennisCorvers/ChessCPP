#include "pch.h"
#include "Logger.hpp"
#include <iomanip>
#include <ctime>

Logger::Logger() :
	m_loggerSeverity(LoggerSeverity::NONE)
{
	initFile();
}

Logger::Logger(LoggerSeverity loggingOptions) :
	m_loggerSeverity(loggingOptions)
{
	initFile();
}

Logger::Logger(unsigned int loggingOptions) :
	m_loggerSeverity(static_cast<LoggerSeverity>(loggingOptions))
{
	initFile();
}

Logger::Logger(const std::string & path) :
	m_loggerSeverity(LoggerSeverity::NONE)
{
	initFile(&path);
}

Logger::Logger(const std::string & path, LoggerSeverity loggingOptions) :
	m_loggerSeverity(loggingOptions)
{
	initFile(&path);
}

Logger::Logger(const std::string & path, unsigned int loggingOptions) :
	m_loggerSeverity(static_cast<LoggerSeverity>(loggingOptions))
{
	initFile(&path);
}

Logger::~Logger() {
	m_logFile.close();
}

void Logger::log(const std::string& message) {
	log(LoggerSeverity::NONE, message);
}

void Logger::log(LoggerSeverity severity, const std::string& message) {
	std::string prefix;

	switch (severity) {
	case LoggerSeverity::DEBUG:
		prefix = "Debug: "; break;
	case LoggerSeverity::ERROR:
		prefix = "[Error]: "; break;
	case LoggerSeverity::FATAL:
		prefix = "[FATAL]: "; break;
	case LoggerSeverity::INFO:
		break;
	case LoggerSeverity::NONE:
		break;
	case LoggerSeverity::TRACE:
		prefix = "[Trace]: "; break;
	case LoggerSeverity::WARNING:
		prefix = "[Warning]: "; break;
	default:
		prefix = ""; break;
	}

	log_inner(severity, message, prefix);
}

void Logger::trace(const std::string & message)
{
	log(LoggerSeverity::TRACE, message);
}

void Logger::debug(const std::string & message)
{
	log(LoggerSeverity::DEBUG, message);
}

void Logger::info(const std::string & message)
{
	log(LoggerSeverity::INFO, message);
}

void Logger::warning(const std::string & message)
{
	log(LoggerSeverity::WARNING, message);
}

void Logger::error(const std::string & message)
{
	log(LoggerSeverity::ERROR, message);
}

void Logger::fatal(const std::string & message)
{
	log(LoggerSeverity::FATAL, message);
}

void Logger::initFile(const std::string* const path) {
	if (path == nullptr) {
		m_logFile.close();
		return;
	}

	m_logFile.open(*path, std::fstream::out | std::fstream::app);

	//if (m_logFile)
	//	std::cerr << "Unable to create log file with specified path. Path: " + *path << std::endl;
}

const std::string Logger::getTime() {
	auto bt = localtime_xp(std::time(0));
	char buf[64];
	return { buf, std::strftime(buf, sizeof(buf), DATETIME_FORMAT.c_str(), &bt) };
}

std::tm Logger::localtime_xp(std::time_t timer)
{
	std::tm bt{};
#if defined(__unix__)
	localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
	localtime_s(&bt, &timer);
#else
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	bt = *std::localtime(&timer);
#endif
	return bt;
}

bool Logger::shouldLog(LoggerSeverity severity) {
	return m_loggerSeverity == LoggerSeverity::NONE || severity == LoggerSeverity::NONE || m_loggerSeverity & severity;
}

void Logger::log_inner(LoggerSeverity severity, const std::string& message, const std::string& prefix)
{
	if (!shouldLog(severity))
		return;

	auto output = getTime() + prefix + message + '\n';
	if (LoggerMode & LogMode::Console)
		std::cout << output;

	if (LoggerMode & LogMode::File) {
		if (m_logFile) {
			const std::lock_guard<std::mutex> lock(m_lock);

			m_logFile << output;
			m_logFile.flush();
		}
	}
}
