//
// Created by Paul Cirstean on 05.12.2023.
//

#include "CdnManager.h"

CdnManager* CdnManager::instance;
const int CDN_THREADS = 15;

CdnManager::CdnManager() {
    cdns.reserve(CDN_NODES);
    cdns.push_back(new CdnThread(0, 8002, 8003, "en"));
    cdns.push_back(new CdnThread(1, 8004, 8005, "fr"));
    cdns.push_back(new CdnThread(2, 8006, 8007, "rus"));
    cdns.push_back(new CdnThread(3, 8008, 8009, "china"));
    cdns.push_back(new CdnThread(4, 8010, 8011, "usa"));
}


CdnThread* CdnManager::GetBestCdnServerToHandleRequest(Location clientLocation) {
    CdnThread* bestThread = nullptr;
    double bestRatio = 1e9;

    for(const auto cdn : cdns) {
        if (cdn->isAlive) {
            auto distance = Location::GetDistance(clientLocation, cdn->location);
            auto load = cdn->load;
            double ratio = CalculateRatio(distance, load);
            if (bestRatio > ratio) {
                bestRatio = ratio;
                bestThread = cdn;
            }
        }
    }

    return bestThread;
}

double CdnManager::CalculateRatio(double distance, int load) {
    // Here may be some improvements.
    return distance + load * 1000;
}