//
// Created by Paul Cirstean on 29.11.2023.
//

#ifndef CLIENT_IRESPONSE_H
#define CLIENT_IRESPONSE_H

#include <sstream>
#include <iostream>

class IResponse {
public: short statusCode;
public: std::string content;
public: IResponse(short statusCode, std::string content) : statusCode(statusCode), content(content) {}
public: IResponse(std::string response) {
        // we know all the responses have 3 digits as in HTTP.
        auto status_string = response.substr(0, 3);
        statusCode = atoi(status_string.c_str());
        content = response.substr(3);

        content.erase(content.begin(), std::find_if(content.begin(), content.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
}
public: std::string GetMessage() const { return std::to_string(statusCode) + " " + content; }
};
#endif //CLIENT_IRESPONSE_H
