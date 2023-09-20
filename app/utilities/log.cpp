#include "log.h"

#include <stdio.h>

#include <string>

#include "main.h"

static Log globalLog{Log::Level::Trace, "GLOB"};

/**
 * @brief A method for write formatted information with printf
 *
 * @param level Log level string
 * @param file Source file path (__FILE__)
 * @param line Source file line (__LINE__)
 * @param format Message format
 * @param args Formatted message parameters
 */
void Log::printf(const char* level, const char* file, int line, const char* format, va_list& args) const {
    ::printf("%08lu [%s] {%s} ", HAL_GetTick(), level, name.c_str());
    if (line >= 0) {
        auto pos = std::string(file).find(EXECUTABLE) + sizeof(EXECUTABLE);
        ::printf("(%s:%d) ", file + pos, line);
    }
    ::vprintf(format, args);
    ::printf("\r\n");
}

Log& Log::global() {
    return globalLog;
}

/**
 * @brief A method for log message with file and line positions
 *
 * @deprecated Use trace() macro instead
 * @param file Source file path (__FILE__)
 * @param line Source file line (__LINE__)
 * @param format Message format
 * @param ... Formatted message parameters
 */
void Log::fileTrace(const char* file, int line, const char* format, ...) const {
    if (level > Log::Level::Trace || Log::Level::Trace < globalLog.level) return;
    va_list args;
    va_start(args, format);
    printf("TRACE", file, line, format, args);
    va_end(args);
}

/**
 * @brief A method for log message with debug information
 *
 * @param format Debug message format
 * @param ... Debug message parameters
 */
void Log::debug(const char* format, ...) const {
#ifndef NDEBUG
    if (level > Log::Level::Debug || Log::Level::Debug < globalLog.level) return;
    va_list args;
    va_start(args, format);
    printf("DEBUG", nullptr, -1, format, args);
    va_end(args);
#endif
}

/**
 * @brief A method for log message with info information
 *
 * @param format Info message format
 * @param ... Info message parameters
 */
void Log::info(const char* format, ...) const {
    if (level > Log::Level::Info || Log::Level::Info < globalLog.level) return;
    va_list args;
    va_start(args, format);
    printf("INFO", nullptr, -1, format, args);
    va_end(args);
}

/**
 * @brief A method for log message with warning information
 *
 * @param format Warning message format
 * @param ... Warning message parameters
 */
void Log::warning(const char* format, ...) const {
    if (level > Log::Level::Warning || Log::Level::Warning < globalLog.level) return;
    va_list args;
    va_start(args, format);
    printf("WARN", nullptr, -1, format, args);
    va_end(args);
}

/**
 * @brief A method for log message with error information
 *
 * @param format Error message format
 * @param ... Error message parameters
 */
void Log::error(const char* format, ...) const {
    if (level > Log::Level::Error || Log::Level::Error < globalLog.level) return;
    va_list args;
    va_start(args, format);
    printf("ERROR", nullptr, -1, format, args);
    va_end(args);
}

template <typename T>
static const char* toBinary(string& container, T value) {
    container.resize(sizeof(T) * 8);
    for (size_t i = 0; i < container.size(); i++) {
        container[i] = (value & (1u << (container.size() - i - 1))) ? '1' : '0';
    }
    return container.c_str();
}

const char* Log::toBinary(string& container, uint8_t value) {
    return ::toBinary(container, value);
}

const char* Log::toHex(string& container, uint8_t* data, uint8_t len) {
    container.resize(len * 3 - 1);
    for (size_t i = 0; i < len; i++) {
        sprintf(&container[i * 3], (i + 1) < len ? "%02X " : "%02X", data[i]);
    }
    return container.c_str();
}
