//
// Created by Paul Cirstean on 04.12.2023.
//

#include "SyncThread.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../Contracts/IRequest.h"
#include "../Contracts/IResponse.h"
#include "../../Shared/Logger/Logger.h"
#include "../../Shared/IO/IO.h"

int SyncThread::number_of_threads;

auto sync_logger = Logger::GetInstance().get();

void WriteToEdgeServer(int client, IResponse* response);

void* SyncThreadAction(void * arg);

void SyncThread::Execute() {
    int id = 1001;
    if (pthread_create(reinterpret_cast<pthread_t *>(&id), NULL, &SyncThreadAction, (void*)&sd) != 0) {
        std::string error_message = std::string("Thread with id {Id} couldn't be created: ") + strerror(errno);
        sync_logger->LogError(error_message.c_str(), AdditionalLogs(), id);
        exit(EXIT_FAILURE);
    }
}
int clientAccept(int* sd) {
    int client;
    struct sockaddr_in from;
    int length = sizeof(from);

    client = accept(*sd, reinterpret_cast<sockaddr *>(&from), reinterpret_cast<socklen_t *>(&length));

    if (client == -1) {
        std::string error_message = std::string("Sync thread couldn't connect to edge server: ") + strerror(errno);
        sync_logger->LogError(error_message.c_str(), AdditionalLogs());
        exit(EXIT_FAILURE);
    }

    char client_ip[36];
    inet_ntop(AF_INET, &from.sin_addr, client_ip, sizeof(client_ip)); // no reason to validate this because we get it from the accept function.
    sync_logger->LogDebug("Sync thread is serving edge server {Address} and port {Port}", "", client_ip, from.sin_port);

    return client;
}

void* SyncThreadAction(void * arg) {
    auto sd = (int*) arg;
    bool isClient = true;
    auto client = clientAccept(sd);

    while(true) {
        if (!isClient) {
            client = clientAccept(sd);
            isClient = true;
        }
        std::string buffer = IO::Read(client);
        if (buffer.size() == 0) {
            std::string error_message =
                    std::string("Sync thread couldn't read from edge-server") ;
            sync_logger->LogError(error_message.c_str(), AdditionalLogs());
            isClient = false;
            continue;
        }

        auto request = IRequest::Create(buffer);
        sync_logger->LogDebug("Sync thread read request {Request}", "", buffer);


        if (request->command != "sync") {
            WriteToEdgeServer(client, new IResponse(400, "Sync channel used for wrong command"));
            continue;
        }

        auto response = new IResponse(200, std::to_string(SyncThread::number_of_threads));
        WriteToEdgeServer(client, response);
    }
}

void WriteToEdgeServer(int client, IResponse* response) {
    if (write (client, response->GetMessage().c_str(), response->GetMessage().size()) <= 0)
    {
        std::string error_message = std::string
                ("Sync thread couldn't write the the response to edge server.") +
                strerror(errno);
        sync_logger->LogError(error_message.c_str(), AdditionalLogs());
        return;
    }

    sync_logger->LogDebug("Sync thread write response {Response}", "", response->GetMessage().c_str());
}