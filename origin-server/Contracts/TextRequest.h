//
// Created by Paul Cirstean on 29.11.2023.
//

#ifndef ORIGIN_SERVER_TEXTREQUEST_H
#define ORIGIN_SERVER_TEXTREQUEST_H

#include "IRequest.h"

class TextRequest : public IRequest {
private: const std::string absolute_path = "/Users/paulcirstean/server-data/text/";
public: const std::string path;
public: TextRequest(std::string path) : path(absolute_path + path) {}

public: HandleRequestResult Handle();
};


#endif //ORIGIN_SERVER_TEXTREQUEST_H
