#pragma once
#include <fstream>
#include <string>
#include <mutex>

enum LoggerSeverity {
	NONE = 0,
	TRACE = 1 << 0,
	DEBUG = 1 << 1,
	INFO = 1 << 2,
	WARNING = 1 << 3,
	ERROR = 1 << 4,
	FATAL = 1 << 5
};

enum LogMode {
	Console = 1 << 0,
	File = 1 << 1,
	Both = Console | File
};

class Logger {
private:
	const std::string DATETIME_FORMAT = "[%d-%m-%Y %H:%M:%S]: ";
	LoggerSeverity m_loggerSeverity;
	std::fstream m_logFile;

	std::mutex m_lock;

public:
	LogMode LoggerMode = LogMode::Both;

	Logger();

	Logger(LoggerSeverity loggingOptions);
	Logger(unsigned int loggingOptions);

	Logger(const std::string& path);
	Logger(const std::string& path, LoggerSeverity loggingOptions);
	Logger(const std::string& path, unsigned int loggingOptions);

	Logger(const Logger& logger) = delete;
	Logger(Logger&& logger) = delete;
	Logger& operator=(Logger&& other) = delete;
	Logger& operator=(const Logger& other) = delete;

	virtual ~Logger();

	void log(const std::string& message);
	void log(LoggerSeverity severity, const std::string& message);

	void trace(const std::string& message);
	void debug(const std::string& message);
	void info(const std::string& message);
	void warning(const std::string& message);
	void error(const std::string& message);
	void fatal(const std::string& message);

private:
	void initFile(const std::string* const path = nullptr);

	const std::string getTime();

	std::tm localtime_xp(std::time_t timer);

	bool shouldLog(LoggerSeverity severity);

	void log_inner(LoggerSeverity severity, const std::string& message, const std::string& prefix = "");
};