//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef SHARED_SERVER_H
#define SHARED_SERVER_H

#include "../Logger/Logger.h"

class Server {
private: const std::shared_ptr<Logger> logger;
private: const int port;
private: int descriptor;
private: void InitilizeServer();
public:
    Server(int port) : port(port), logger(Logger::GetInstance()) {
        InitilizeServer();
    }
    const int Descriptor() const;
};


#endif //SHARED_SERVER_H
