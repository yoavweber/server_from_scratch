#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <iomanip>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>

#ifndef ThreadPool_H // include guard
#define ThreadPool_H

typedef void (*Connection)(int);

class ThreadPool
{
public:
    // try to remove the explicit and see whats going on
    explicit ThreadPool(size_t numThreads, int socket)
    {
        start(numThreads, socket);
    }
    ~ThreadPool()
    {
        stop();
    }

    // adding tasks to the queue
    void enqueuq(Connection task);

private:
    std::vector<std::thread> threads;

    std::condition_variable eventVar;

    std::mutex eventMutex;

    bool stopping = false;
    // hold the list of task to be executed on the thread pool
    std::queue<Connection> tasks;
    void start(size_t numThreads, int socket);
    // tell the program not to throw an exception and if it does just let the program crash
    void stop();
};

#endif