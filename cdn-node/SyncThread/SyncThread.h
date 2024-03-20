//
// Created by Paul Cirstean on 04.12.2023.
//

#ifndef CDN_NODE_SYNCTHREAD_H
#define CDN_NODE_SYNCTHREAD_H
#include <iostream>

class SyncThread {
public: static int number_of_threads;
private: const int sd;
public: SyncThread(int sd) : sd(sd) {}
public: void Execute();
public: static void IncrementThreads() { ++number_of_threads; }
public: static void DecrementThreads() { --number_of_threads; }
};

#endif //CDN_NODE_SYNCTHREAD_H
