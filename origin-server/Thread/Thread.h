//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef ORIGIN_SERVER_THREAD_H
#define ORIGIN_SERVER_THREAD_H

#include <csignal>
#include <unistd.h>
#include <memory>

class Thread {
private: int id;
private: const int sd;
public: Thread(int id, int sd) : id(id), sd(sd) {}
public: void Execute();

};


#endif //ORIGIN_SERVER_THREAD_H
