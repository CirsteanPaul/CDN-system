//
// Created by Paul Cirstean on 04.12.2023.
//

#include "ThreadPool.h"
#include "../CdnManager/CdnManager.h"

void ThreadPool::CreateThreads(int sd) {
    auto cdn_manager = CdnManager::GetInstance();
    logger.get()->LogDebug("Thread pool started creating...", "");
    for (auto cdn : cdn_manager->cdns) {
        cdn->Execute();
    }
    for (int i = 0; i < pool_size ;i ++) {
        threads[i] = new Thread(i, sd);
        threads[i]->Execute();
    }

    logger.get()->LogDebug("Threads are waiting for requests", "");

    while(true) {
        pause();
    }
}

