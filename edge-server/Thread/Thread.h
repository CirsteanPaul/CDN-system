//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef EDGE_SERVER_THREAD_H
#define EDGE_SERVER_THREAD_H

#include <csignal>
#include <unistd.h>
#include <memory>

class Thread {
private: int id;
private: const int sd;
public: Thread(int id, int sd) : id(id), sd(sd) {}
public: void Execute();

};


#endif //EDGE_SERVER_THREAD_H
