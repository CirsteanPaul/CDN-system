//
// Created by Paul Cirstean on 29.11.2023.
//

#ifndef ORIGIN_SERVER_IRESPONSE_H
#define ORIGIN_SERVER_IRESPONSE_H

#include <iostream>

class IResponse {
public: short statusCode;
public: std::string content;
public: IResponse(short statusCode, std::string content) : statusCode(statusCode), content(content) {}
public: std::string GetMessage() const { return std::to_string(statusCode) + " " + content; }
};
#endif //ORIGIN_SERVER_IRESPONSE_H
