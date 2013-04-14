#include "Logging.h"
#include <sys/time.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace mynet;

/* 
 * The format of a log message:
 * date time file line level(if need) fucn(if exist) message
 */


void Logger::logTime()
{
	struct timeval tv;
	struct tm tm1;
	char timeStr[50];
	gettimeofday(&tv, NULL);

	localtime_r(&tv.tv_sec, &tm1);
	memset(timeStr, 0, sizeof(timeStr)); 
	sprintf(timeStr, "%4.4d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d:%6.6ld",
				tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
				tm1.tm_hour, tm1.tm_min,tm1.tm_sec, tv.tv_usec);
	stream_ << timeStr << ' ';
}

void Logger::logFileAndLine(const char *file, int line)
{
	stream_ << file << ' ' << line << ' ';
}

void Logger::logLevel(int level)
{
	char levelStrs[NUM_LOG_LEVELS][10] = {
		"TRACE",
		"DEBUG",
		"INFO",
		"WARN",
		"ERROR",
		"FATAL"
	};
		
	stream_ << levelStrs[level] << ' ';
}

void Logger::logFunc(const char *file) 
{
	stream_ << file << ' ';
}

Logger::Logger(const char *file, int line)
{
	logTime();
	logFileAndLine(file, line);
	level_ = INFO;
}

Logger::Logger(const char *file, int line, Log_Level level)
{
	logTime();
	logFileAndLine(file, line);
	logLevel(level);
	level_ = level;
}

Logger::Logger(const char *file, int line, Log_Level level, 
			const char *func) 
{
	logTime();
	logFileAndLine(file, line);
	logLevel(level);
	logFunc(func);
	level_ = level;
}

std::ostream& Logger::stream()
{
	return stream_;
}

Logger::~Logger() 
{
	stream_ << std::endl;
	std::string buf(stream_.str());
	::write(1, buf.c_str(), buf.size());
	if (level_ == FATAL) {
		::abort();
	}
}
