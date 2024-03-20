//
// Created by Paul Cirstean on 04.12.2023.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../Shared/Logger/Logger.h"
#include "Thread.h"
#include "../Contracts/IResponse.h"
#include "../Client/Client.h"
#include "../Cache/Cache.h"
#include "../Contracts/IRequest.h"
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
FileType GetTypeOfFile(std::string command);

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
        SyncThread::IncrementThreads();
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

        SyncThread::DecrementThreads();

        logger->LogDebug("Thread with id {Id} responded to client with address {Address} and port {Port} was {Message}"
        , "", thread_info->id, client_ip, from.sin_port, response->GetMessage());
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
    auto cache = Cache::GetInstance().get();
    FileType file_type = GetTypeOfFile(request->command);
    auto file = cache->CheckCache(request->payload, file_type);

    if (file) {
        logger->LogDebug("Thread {ThreadId} hits cache for the file {FileName}", "", thread_id, request->payload);
        return new IResponse(200, file->content);
    }
    // cache missed so we will call the origin server.

    auto origin_server_client = new Client(LOCALHOST, ORIGIN_PORT);
    int code = origin_server_client->Connect();
    if (code != 0) {
        logger->LogError("Thread {ThreadId} couldn't connect to origin", AdditionalLogs(), thread_id);
        return new IResponse(500, "Internal server error");
    }

    if (write(origin_server_client->GetDescriptor(), buffer.c_str(), buffer.size()) <= 0) {
        logger->LogError("Thread {ThreadId} couldn't write to origin", AdditionalLogs(), thread_id);
        return new IResponse(500, "Internal server error");
    }

    buffer = IO::Read(origin_server_client->GetDescriptor());
    if (buffer.size() == 0) {
        logger->LogError("Thread {ThreadId} couldn't read from origin", AdditionalLogs(), thread_id);
        return new IResponse(500, "Internal server error");
    }
    auto response = new IResponse(buffer);

    if (close(origin_server_client->GetDescriptor()) == -1) {
        std::string error_message = std::string
                                            ("Thread with id {Id} couldn't close the connection with client with address {Address} and port {Port}: ") +
                                    strerror(errno);
        logger->LogError(error_message.c_str(), AdditionalLogs(), thread_id);
        return new IResponse(500, "Internal server error");
    }
    // if response succeeded we add the new file to cache.
    if(response->statusCode == 200) {
        logger->LogDebug("Thread {ThreadId} added to cache the file {FileName}", "", thread_id, request->payload);
        cache->AddItemToCache(request->payload, response->content, file_type);
    }

    return response;
}

FileType GetTypeOfFile(std::string command) {
    if (command == "get-text-file") {
        return TextFile;
    }
    if (command == "get-image-file") {
        return ImageFile;
    }

    return FileType::None;

}
