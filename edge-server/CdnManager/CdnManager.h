//
// Created by Paul Cirstean on 05.12.2023.
//

#ifndef EDGE_SERVER_CDNMANAGER_H
#define EDGE_SERVER_CDNMANAGER_H

#include "../CdnThread/CdnThread.h"

class CdnManager {
public: std::vector<CdnThread*> cdns;
public: const int CDN_NODES = 5;
private: static CdnManager* instance;
private: CdnManager();
private: static double CalculateRatio(double, int);
public: static CdnManager* GetInstance() {
    if (instance == nullptr) {
        instance = new CdnManager();
    }

    return instance;
}
public: CdnThread* GetBestCdnServerToHandleRequest(Location);
};

#endif //EDGE_SERVER_CDNMANAGER_H
