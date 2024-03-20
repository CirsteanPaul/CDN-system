#include <iostream>
#include "../Shared/Server/Server.h"
#include "ThreadPool/ThreadPool.h"

int edge_server_port = 8080;
int main() {

    auto logger = Logger::GetInstance().get();
    logger->setPrefix("EDGE-SERVER");
    logger->SetLogPreferences("", LogLevel::INFO, LogOutput::CONSOLE);

    auto server = new Server(edge_server_port);

    auto thread_pool = new ThreadPool(15);
    thread_pool->CreateThreads(server->Descriptor());
}
