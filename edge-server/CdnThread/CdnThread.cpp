//
// Created by Paul Cirstean on 05.12.2023.
//

#include <unistd.h>
#include "CdnThread.h"
#include "../../Shared/Logger/Logger.h"
#include "../Client/Client.h"
#include "../Contracts/IResponse.h"
#include "../../Shared/IO/IO.h"

auto cdn_logger = Logger::GetInstance().get();

void* CdnThreadAction(void * arg);

void CdnThread::Execute() {
    int id_thread = id;
    if (pthread_create(reinterpret_cast<pthread_t *>(&id_thread), NULL, &CdnThreadAction, (void*)this) != 0) {
        std::string error_message = std::string("Thread with id {Id} couldn't be created: ") + strerror(errno);
        cdn_logger->LogError(error_message.c_str(), AdditionalLogs(), id);
        exit(EXIT_FAILURE);
    }
}

void* CdnThreadAction(void * arg) {
    auto cdn_connection = (CdnThread*) arg;
    auto client = new Client(LOCALHOST, cdn_connection->sync_port);
    bool warning_shown = false;
    while(true) {

        if (!cdn_connection->isAlive) {
            int result = client->Connect();
            if (result == -1) {
                perror("Cdn thread socket issues");
                exit(EXIT_FAILURE);
            }
            if (result == -2) {
                if (!warning_shown) {
                    cdn_logger->LogError("{CdnThreadId} cdn server {PORT} is dead.",
                                        AdditionalLogs(), cdn_connection->id, cdn_connection->port);
                    warning_shown = true;
                }
                sleep(3);
                continue;
            }

            cdn_connection->isAlive = true;
            cdn_logger->LogWarn("Cdn thread {CdnThreadId} is connected to cdn server {PORT}", AdditionalLogs(), cdn_connection->id, cdn_connection->port);
        }

        char buff[256] = "sync";
        if (write(client->GetDescriptor(), buff, sizeof(buff)) < 0) {
            // The connection is lost.
            cdn_logger->LogDebug("Cdn thread {CdnThreadId} lost connection when writing with cdn server {PORT}", AdditionalLogs(), cdn_connection->id, cdn_connection->port);
            cdn_connection->isAlive = false;
            continue;
        }

        std::string buffer = IO::Read(client->GetDescriptor());

        if (buffer.size() == 0) {
            // The connection is lost.
            cdn_logger->LogDebug("Cdn thread {CdnThreadId} lost connection when reading with cdn server {PORT}", AdditionalLogs(), cdn_connection->id, cdn_connection->port);
            cdn_connection->isAlive = false;
            continue;
        }

        auto response = new IResponse(buffer);

        if (response->statusCode == 200) {
            cdn_connection->load = atoi(response->content.c_str());
        }

        cdn_logger->LogDebug("Cdn thread {CdnThreadId} got number of threads {Load} used by cdn server {PORT}", AdditionalLogs(), cdn_connection->id, cdn_connection->load, cdn_connection->port);

        sleep(3);
    }
}