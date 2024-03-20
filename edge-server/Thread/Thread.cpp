//
// Created by Paul Cirstean on 04.12.2023.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Thread.h"
#include "../../Shared/Logger/Logger.h"
#include "../Contracts/IResponse.h"
#include "../Contracts/IRequest.h"
#include "../CdnManager/CdnManager.h"
#include "../../Shared/IO/IO.h"
#include <sstream>

auto logger = Logger::GetInstance().get();
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

struct ThreadInfo {
    int id;
    int sd;
};
void* ThreadAction(void*);
const IResponse* ServerAction(int, int);

void Thread::Execute() {
    auto info = new ThreadInfo();
    info->id = id;
    info->sd = sd;

    if (pthread_create(reinterpret_cast<pthread_t *>(&id), NULL, &ThreadAction, (void*)info) != 0) {
        std::string error_message = std::string("Thread with id {Id} couldn't be created: ") + strerror(errno);
        logger->LogError(error_message.c_str(), AdditionalLogs(), id);
        exit(EXIT_FAILURE);
    }
}

void* ThreadAction(void * arg) {
    int client;
    struct sockaddr_in from;
    auto thread_info = (ThreadInfo*) arg;
    int length = sizeof(from);

    while(true)
    {
        pthread_mutex_lock(&mlock);
        client = accept(thread_info->sd, reinterpret_cast<sockaddr *>(&from), reinterpret_cast<socklen_t *>(&length));

        if (client == -1) {
            std::string error_message = std::string("Thread with id {Id} couldn't connect to client: ") + strerror(errno);
            logger->LogError(error_message.c_str(), AdditionalLogs(), thread_info->id);
            pthread_mutex_unlock(&mlock);
            continue;
        }

        pthread_mutex_unlock(&mlock);
        char client_ip[36];
        inet_ntop(AF_INET, &from.sin_addr, client_ip, sizeof(client_ip)); // no reason to validate this because we get it from the accept function.
        logger->LogDebug("Thread with id {Id} is serving client address {Address} and port {Port}", "", thread_info->id, client_ip, from.sin_port);

        auto response = ServerAction(client, thread_info->id);

        if (write (client, response->GetMessage().c_str(), response->GetMessage().size()) <= 0)
        {
            std::string error_message = std::string
                    ("Thread with id {Id} couldn't write the the response to client with address {Address} and port {Port}: ") +
                    strerror(errno);
            logger->LogError(error_message.c_str(), AdditionalLogs(), thread_info->id, client_ip, from.sin_port);
            continue;
        }

        if (close(client) == -1) {
            std::string error_message = std::string
                                                ("Thread with id {Id} couldn't close the connection with client with address {Address} and port {Port}: ") +
                                        strerror(errno);
            logger->LogError(error_message.c_str(), AdditionalLogs(), thread_info->id, client_ip, from.sin_port);
            continue;
        }

        logger->LogInformation("Thread with id {Id} responded to client with address {Address} and port {Port} with {Message}"
                , "", thread_info->id, client_ip, from.sin_port, response->GetMessage());

        delete response;
    }
}

const IResponse* ServerAction(int client, int thread_id)
{
    std::string buffer = IO::Read(client);
    if (buffer.size() == 0) {
        std::string error_message = std::string("Thread {ThreadId} couldn't read from client {ClientId}: ") + strerror(errno);
        logger->LogError(error_message.c_str(), AdditionalLogs(), thread_id, client);
        return new IResponse(500, "Connection was faulty");
    }

    logger->LogInformation("Thread {ThreadId} received the request {Request}", ""
            , thread_id, buffer);

    IRequest* request = IRequest::Create(buffer);

    auto cdn_manager = CdnManager::GetInstance();
    auto bestCdnRedirect = cdn_manager->GetBestCdnServerToHandleRequest(request->location);
    if (bestCdnRedirect == nullptr) {
        return new IResponse(503, "Service is unavailable.");
    }

    return new IResponse(302, std::to_string(bestCdnRedirect->port));;
}

