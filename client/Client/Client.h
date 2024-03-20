//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "../../Shared/Logger/Logger.h"

const std::string LOCALHOST = "127.0.0.1";
const int ORIGIN_PORT = 8001;

class Client {
private: int sd;
private: const std::string address;
private: const int port;
private: Logger* logger;
public: Client(std::string address, int port) : port(port), address(address), logger(nullptr) {}
public: const int GetDescriptor() const;
public: int Connect();
};


#endif //CLIENT_CLIENT_H
