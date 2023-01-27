#pragma once
#include "pch.h"

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_FATAL 10
#define LOG_LEVEL_ERROR 20
#define LOG_LEVEL_WARN 30
#define LOG_LEVEL_INFO 40
#define LOG_LEVEL_DEBUG 50
#define LOG_LEVEL_TRACE 60
#define LOG_LEVEL_ALL 100
#define __LOG_FILE__ "log.txt"

#define fatal(str, ...) writeLog(__FUNCTION__, __LINE__, LOG_LEVEL_FATAL, str, __VA_ARGS__)
#define error(str, ...) writeLog(__FUNCTION__, __LINE__, LOG_LEVEL_ERROR, str, __VA_ARGS__)
#define warn(str, ...) writeLog(__FUNCTION__, __LINE__, LOG_LEVEL_WARN, str, __VA_ARGS__)
#define info(str, ...) writeLog(__FUNCTION__, __LINE__, LOG_LEVEL_INFO, str, __VA_ARGS__)
#define debug(str, ...) writeLog(__FUNCTION__, __LINE__, LOG_LEVEL_DEBUG, str, __VA_ARGS__)
#define trace(str, ...) writeLog(__FUNCTION__, __LINE__, LOG_LEVEL_TRACE, str, __VA_ARGS__)

class Logger {
private:
    int logLevel;
    bool isOutput;

private:
    std::string getTimeStamp();

public:
    Logger();
    Logger(int level);
    void writeLog(const char* funcName, int line, int level, const char* str, ...);
};

static Logger g_Log(LOG_LEVEL_WARN);

