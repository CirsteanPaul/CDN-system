#include "Logger.h"

std::shared_ptr<Logger> Logger::loggerInstance;

void Logger::SetLogPreferences(std::string logFileName = "",
                               LogLevel level = LogLevel::ERROR,
                               LogOutput output = LogOutput::CONSOLE) {
    logLevel = level;
    logOutput = output;

    if (logOutput == LogOutput::FILE && !logFileName.empty()) {
        logFile.open(logFileName, std::ios::app);
        if (!logFile.good()) {
            std::cerr << "Can't Open Log File" << std::endl;
            logOutput = LogOutput::CONSOLE;
        }
    }
}

const std::shared_ptr<Logger> Logger::GetInstance() {
    if (loggerInstance == nullptr) {
        loggerInstance = std::shared_ptr<Logger>(new Logger());
    }

    return loggerInstance;
}

void Logger::PrintMessage(std::string color, std::string message) {
    const std::string reset("\033[0m");
    mut.lock();
    if (logOutput == LogOutput::CONSOLE) {
        std::cout << color << message << reset;
        fflush(stdout);
    } else {
        logFile << message;
        logFile.flush();
    }
    mut.unlock();
}