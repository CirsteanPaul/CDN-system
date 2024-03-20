//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef EDGE_SERVER_THREADPOOL_H
#define EDGE_SERVER_THREADPOOL_H

#include "../Thread/Thread.h"
#include <iostream>
#include "../../Shared/Logger/Logger.h"

class ThreadPool {
private: const int pool_size;
private: const std::shared_ptr<Logger> logger;
private: std::vector<Thread*> threads;
public: ThreadPool(int pool_size) : pool_size(pool_size), logger(Logger::GetInstance()) {
    threads.reserve(pool_size);
}
public: void CreateThreads(int sd);
};


#endif //EDGE_SERVER_THREADPOOL_H
