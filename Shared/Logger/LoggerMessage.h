 //
// Created by Paul Cirstean on 27.10.2023.
//

#ifndef SHARED_LOGGERMESSAGE_H
#define SHARED_LOGGERMESSAGE_H

#include <iostream>
#include <time.h>

class LoggerMessage {
private: int variable_count = 0;
private: std::vector<std::string> names;
private: std::string logger_message;
private:
    int countBracePairs(const std::string& input) {
        int count = 0;
        int openBraceCount = 0;
        std::string aux = "";

        for (char ch : input) {
            aux += ch;
            if (ch == '{') {
                openBraceCount++;
                aux = "";
            } else if (ch == '}' && openBraceCount > 0) {
                count++; // Found a matching pair
                aux.pop_back();
                names.push_back(aux);
                openBraceCount--;
            }
        }

        return count;
    }

    template <typename... Args>
    void print_variadic_args(std::string value,  const Args&... args) {
        logger_message = logger_message + names[variable_count] + ": " + value + "\n  ";
        variable_count ++;
        print_variadic_args(args...);
    }

    template <typename... Args>
    void print_variadic_args(const char* value,  const Args&... args) {
        logger_message = logger_message + names[variable_count] + ": " + value + "\n  ";
        variable_count ++;
        print_variadic_args(args...);
    }

    template <typename... Args>
    void print_variadic_args(bool value,  const Args&... args) {
        auto message = value ? "True" : "False";
        logger_message = logger_message + names[variable_count] + ": " + message + "\n  ";
        variable_count ++;
        print_variadic_args(args...);
    }
    template <typename T, typename... Args>
    void print_variadic_args(const T& value, const Args&... args) {
        logger_message += names[variable_count] + ": " + std::to_string(value) + "\n  ";
        variable_count ++;
        print_variadic_args(args...);
    }

    void print_variadic_args() {}

public:
    template<typename... Args>
    static std::string LogMessage(const char * message, const Args&...args) {
        auto logger = new LoggerMessage();
        logger->variable_count = 0;
        logger->logger_message.clear();
        logger->logger_message = message;

        int braces = logger->countBracePairs(message);
        std::string value_string = braces > 1 ? "Values" : "Value";
        logger->logger_message += "\n" + value_string + ": \n{\n  ";
        logger->print_variadic_args(args...);

        // remove whitespaces
        logger->logger_message.pop_back();
        logger->logger_message.pop_back();
        logger->logger_message += "}";
        if (logger->variable_count != braces) {
            std::cout << "Error message is wrong";
            exit(EXIT_FAILURE);
        }

        return logger->logger_message;
    }

    static const std::string GetCurrentDateTime() {
        time_t now = time(NULL);
        char buf[80];
        auto time = std::localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", time);
        return buf;
    }
};
#endif //SHARED_LOGGERMESSAGE_H
