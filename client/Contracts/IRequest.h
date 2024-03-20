//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef CLIENT_IREQUEST_H
#define CLIENT_IREQUEST_H

#include <sstream>
#include <iostream>

class IRequest {
public:
    std::string command;
public:
    std::string payload;
public: std::string location;
public:
    static IRequest *Create(std::string request) {
        IRequest* request_object = new IRequest();
        std::istringstream is(request);
        std::vector<std::string> request_tokens;
        std::string request_token;
        while (is >> request_token) {
            if (!request_token.empty()) {
                request_tokens.push_back(request_token);
            }
        }

        request_object->command = request_tokens[0];
        request_object->location = request_tokens[2];
        request_object->payload = request_tokens[1];

        return request_object;
    }
public: std::string GetLocatedMessage() {
        return command + " " + payload + " " + location;
    }
public: std::string GetMessage() {
        return command + " " + payload;
    }
};

#endif //CLIENT_IREQUEST_H
