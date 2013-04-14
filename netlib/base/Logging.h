#ifndef MYNET_LOGGING_H
#define MYNET_LOGGING_H

#include <sstream>
namespace mynet {

class Logger {
public:
	enum Log_Level {
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
		NUM_LOG_LEVELS,
	};

	Logger(const char *file, int line);
	Logger(const char *file, int line, Log_Level level);
	Logger(const char *file, int line, Log_Level level, const char *func);
	//Logger(const char *file, int line, bool abort);
	~Logger();

	std::ostream& stream();
private:
	void logTime();
	void logFileAndLine(const char *file, int line);
	void logLevel(int level);
	void logFunc(const char *file);
	std::ostringstream stream_;
	int level_;
};

#define LOG_TRACE Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()
#define LOG_INFO Logger(__FILE__, __LINE__).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN, __func__).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR, __func__).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL, __func__).stream()
}
#endif
