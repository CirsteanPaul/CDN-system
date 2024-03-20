//
// Created by Paul Cirstean on 04.12.2023.
//

#include "ThreadPool.h"

const int origin_server_port = 8001;
const int edge_server_port = 8080;
void ThreadPool::CreateThreads(int sd, int sync_sd) {
    logger.get()->LogDebug("Thread pool started creating...", "");

    syncThread = new SyncThread(sync_sd);
    syncThread->Execute();
    for (int i = 0; i < pool_size ;i ++) {
        threads[i] = new Thread(i, sd);
        threads[i]->Execute();
    }
    logger.get()->LogDebug("Threads are waiting for requests", "");

    while(true) {
        pause();
    }
}

