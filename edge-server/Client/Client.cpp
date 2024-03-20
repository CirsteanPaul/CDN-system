//
// Created by Paul Cirstean on 04.12.2023.
//

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Client.h"

const int Client::GetDescriptor() const {
    return sd;
}

int Client::Connect() {
    struct sockaddr_in origin_server;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        std::string error_message = std::string("Cannot connect to cdn server: ") + strerror(errno);
        logger->LogError(error_message.c_str(), "");
        return -1;
    }

    origin_server.sin_family = AF_INET;
    origin_server.sin_addr.s_addr = inet_addr(address.c_str());
    origin_server.sin_port = htons(port);

    if (connect (sd, (struct sockaddr *) &origin_server, sizeof (struct sockaddr)) == -1)
    {
        return -2;
    }

    return 0;
}
