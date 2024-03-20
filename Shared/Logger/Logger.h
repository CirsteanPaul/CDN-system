//
// Created by Paul Cirstean on 27.10.2023.
//

#ifndef SHARED_LOGGER_H
#define SHARED_LOGGER_H
#define AdditionalLogs() \
    []() -> std::string { \
    std::string path(__FILE__); \
    size_t lastSlash = path.find_last_of("/\\"); \
    return "Located in: " + path.substr(lastSlash + 1) + " line: " + std::to_string(__LINE__); \
}()

#include <string>
#include <iostream>
#include <memory>
#include <time.h>
#include <fstream>
#include <cstdarg>
#include "../Result.h"
#include "LoggerMessage.h"

enum class LogLevel {
    ERROR = 1,
    WARN = 2,
    INFO = 3,
    DEBUG = 4
};

enum class LogOutput {
    CONSOLE,
    FILE
};

/**
* Logger Class Used to Output Details of Current Application Flow
*/
class Logger {
private:
    std::mutex mut;
    std::string prefix;
    LogLevel logLevel;
    LogOutput logOutput;
    std::ofstream logFile;

    static std::shared_ptr<Logger> loggerInstance;

public:
    static const std::shared_ptr<Logger> GetInstance();

    void SetLogPreferences(std::string logFileName,
                           LogLevel level,
                           LogOutput output);
    void PrintMessage(std::string, std::string message);
    void setPrefix(std::string prefix) { this->prefix = prefix; }

public:
    template<typename ...Args>
    void LogInformation(const char* message_template, std::string additional_info = "", const Args&...args) {
        auto message = LoggerMessage::LogMessage(message_template, args...);
        const std::string blue("\033[0;34m");

        if (LogLevel::INFO > logLevel) {
            return;
        }

        auto log = "[" + prefix + "] [INFO " + LoggerMessage::GetCurrentDateTime() + "]: " + message + '\n';

        PrintMessage(blue, log);
    }
    void LogInformation(const char* message_template, std::string additional_info = "") {
        const std::string blue("\033[0;34m");

        if (LogLevel::INFO > logLevel) {
            return;
        }

        auto log = "[" + prefix + "] [INFO " + LoggerMessage::GetCurrentDateTime() + "]: " + message_template + '\n';

        PrintMessage(blue, log);
    }

    template<typename ...Args>
    void LogError(const char* message_template, std::string additional_info = "", const Args&...args) {
        auto message = LoggerMessage::LogMessage(message_template, args...);
        const std::string red("\033[0;31m");

        if (LogLevel::ERROR > logLevel) {
            return;
        }

        auto log = "[" + prefix + "] [ERROR " + LoggerMessage::GetCurrentDateTime() + " " + additional_info + "]: " + message + '\n';

        PrintMessage(red, log);
    }
    void LogError(const char* message_template, std::string additional_info = "") {
        const std::string red("\033[0;31m");

        if (LogLevel::ERROR > logLevel) {
            return;
        }

        auto log = "[" + prefix + "] [ERROR " + LoggerMessage::GetCurrentDateTime() + " " + additional_info + "]: " + message_template + '\n';

        PrintMessage(red, log);
    }

    template<typename ...Args>
    void LogWarn(const char* message_template, std::string additional_info = "", const Args&...args) {
        auto message = LoggerMessage::LogMessage(message_template, args...);
        const std::string yellow("\033[1;33m");

        if (LogLevel::WARN > logLevel) {
            return;
        }

        auto log = "[" + prefix + "] [WARN " + LoggerMessage::GetCurrentDateTime() + "]: " + message + '\n';

        PrintMessage(yellow, log);
    }
    void LogWarn(const char* message_template, std::string additional_info = "") {
        const std::string yellow("\033[1;33m");

        if (LogLevel::WARN > logLevel) {
            return;
        }

        auto log = "[" + prefix + "] [WARN " + LoggerMessage::GetCurrentDateTime() + "]: " + message_template + '\n';

        PrintMessage(yellow, log);
    }

    template<typename ...Args>
    void LogDebug(const char* message_template, std::string additional_info = "", const Args&...args) {
        auto message = LoggerMessage::LogMessage(message_template, args...);
        const std::string magenta("\033[0;35m");

        if (LogLevel::DEBUG > logLevel) {
            return;
        }

        auto log = "[" + prefix + "] [DEBUG " + LoggerMessage::GetCurrentDateTime() + " " + additional_info + "]: " + message + '\n';

        PrintMessage(magenta, log);
    }
    void LogDebug(const char* message_template, std::string additional_info = "") {
        const std::string magenta("\033[0;35m");

        if (LogLevel::DEBUG > logLevel) {
            return;
        }

        auto log = "[" + prefix + "] [DEBUG " + LoggerMessage::GetCurrentDateTime() + " " + additional_info + "]: " + message_template + '\n';

        PrintMessage(magenta, log);
    }
};

#endif //SHARED_LOGGER_H
