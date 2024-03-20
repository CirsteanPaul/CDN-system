#include <iostream>
#include "../Shared/Server/Server.h"
#include "ThreadPool/ThreadPool.h"

int CDN_PORT = 8002;

int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cout << "Usage " << argv[0] << " port ";
        exit(EXIT_FAILURE);
    }

    CDN_PORT = atoi(argv[1]);
    int CDN_SYNC_PORT = CDN_PORT + 1;
    auto logger = Logger::GetInstance().get();
    logger->setPrefix("CDN-NODE-" + std::to_string(CDN_PORT));
    logger->SetLogPreferences("", LogLevel::INFO, LogOutput::CONSOLE);
    auto server = new Server(CDN_PORT);
    auto sync_server = new Server(CDN_SYNC_PORT);
    auto thread_pool = new ThreadPool(15);
    thread_pool->CreateThreads(server->Descriptor(), sync_server->Descriptor());
}
