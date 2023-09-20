#ifndef IG_LOG_H
#define IG_LOG_H

#include <stdarg.h>

#include <cstdint>
#include <string>

using std::string;

class Log {
   private:
    void printf(const char* level, const char* file, int line, const char* format, va_list& args) const;

   public:
    static Log& global();

    enum class Level { Trace, Debug, Info, Warning, Error, Off };
    Level level = Level::Debug;
    string name = "NAN";

    void fileTrace(const char* file, int line, const char* format, ...) const;
    void debug(const char* format, ...) const;
    void info(const char* format, ...) const;
    void warning(const char* format, ...) const;
    void error(const char* format, ...) const;

    static const char* toBinary(string& container, uint8_t value);
    static const char* toHex(string& container, uint8_t* data, uint8_t len);

#define trace(...) fileTrace(__FILE__, __LINE__, __VA_ARGS__)
};

#endif  // IG_LOG_H