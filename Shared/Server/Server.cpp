//
// Created by Paul Cirstean on 04.12.2023.
//

#include "Server.h"

#include <netinet/in.h>
#include <cerrno>
#include <cstdlib>

void Server::InitilizeServer() {
    int on = 1;
    struct sockaddr_in server;
    descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (descriptor == -1) {
        std::string error_message = std::string("Socket failed creation: ") + strerror(errno);
        logger->LogError(error_message.c_str(), AdditionalLogs());
        exit(EXIT_FAILURE);
    }

    if (setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        std::string error_message = std::string("Socket options failed to be set: ") + strerror(errno);
        logger->LogError(error_message.c_str(), AdditionalLogs());
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if (bind (descriptor, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        std::string error_message = std::string("Socket couldn't be bind: ") + strerror(errno);
        logger->LogError(error_message.c_str(), AdditionalLogs());
        exit(EXIT_FAILURE);
    }

    if (listen (descriptor, 2) == -1)
    {
        std::string error_message = std::string("Listen is not working: ") + strerror(errno);
        logger->LogError(error_message.c_str(), AdditionalLogs());
        exit(EXIT_FAILURE);
    }

    logger->LogInformation("Server is listening on port {PORT} ...", "", port);
}

const int Server::Descriptor() const {
    return descriptor;
}