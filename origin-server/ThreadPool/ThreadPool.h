//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef ORIGIN_SERVER_THREADPOOL_H
#define ORIGIN_SERVER_THREADPOOL_H

#include "../Thread/Thread.h"
#include "../../Shared/Logger/Logger.h"
#include <iostream>

class ThreadPool {
private: const int pool_size;
private: const std::shared_ptr<Logger> logger;
private: std::vector<Thread*> threads;
public: ThreadPool(int pool_size) : pool_size(pool_size), logger(Logger::GetInstance()) {
    threads.reserve(pool_size);
}
public: void CreateThreads(int sd);
};


#endif //ORIGIN_SERVER_THREADPOOL_H
