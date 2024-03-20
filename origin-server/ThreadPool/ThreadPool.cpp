//
// Created by Paul Cirstean on 04.12.2023.
//

#include "ThreadPool.h"

void ThreadPool::CreateThreads(int sd) {
    logger.get()->LogDebug("Thread pool started creating...", "");
    for (int i = 0; i < pool_size ;i ++) {
        threads[i] = new Thread(i, sd);
        threads[i]->Execute();
    }

    logger.get()->LogDebug("Threads are waiting for requests", "");

    while(true) {
        pause();
    }
}

