//
// Created by Paul Cirstean on 05.12.2023.
//

#ifndef EDGE_SERVER_CDNTHREAD_H
#define EDGE_SERVER_CDNTHREAD_H

#include <iostream>
#include "../../Shared/Location/Location.h"

class CdnThread {
public: int id;
public: int sd;
public: const int port;
public: const int sync_port;
public: int load;
public: Location location;
public: int isAlive;
public: static std::vector<CdnThread*> cdn_threads;
public: CdnThread(int id, int port, int sync_port, std::string location) : id(id), port(port), sync_port(sync_port), location(Location(location)) {}
public: void Execute();
};


#endif //EDGE_SERVER_CDNTHREAD_H
