#include <string>
#include "ThreadPool/ThreadPool.h"
#include "../Shared/Server/Server.h"

int main() {

    auto logger = Logger::GetInstance();
    logger.get()->SetLogPreferences("", LogLevel::DEBUG, LogOutput::CONSOLE);
    logger.get()->setPrefix("ORIGIN");

    auto server = new Server(8001);

    auto thread_pool = new ThreadPool(10);
    thread_pool->CreateThreads(server->Descriptor());
}