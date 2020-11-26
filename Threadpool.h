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

using Connection = void (*)(int);

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
    inline void enqueuq(Connection task)
    {
        // why are we creating another scope?
        {
            std::unique_lock<std::mutex> lock{eventMutex};
            // insert element to the queue, not sure why using the move
            tasks.emplace(std::move(task));
        }
        // notifying one of the threads to wake up and take the job
        eventVar.notify_one();
    }

private:
    std::vector<std::thread> threads;

    std::condition_variable eventVar;

    std::mutex eventMutex;

    bool stopping = false;
    // hold the list of task to be executed on the thread pool
    std::queue<Connection> tasks;
    void start(size_t numThreads, int socket)
    {
        for (int i = 0; i < numThreads; i++)
        {
            threads.emplace_back([=] {
                while (true)
                {

                    Connection task;
                    // we created another scope cause we don't want the mutex to be lock while the task is executing(11:30)
                    {
                        // looking the thread from modifing data(why do we need to lock if we only read the data and not modifying it)
                        std::unique_lock<std::mutex> lock{eventMutex};
                        // the event varible would wake up if we are stopping or if there is task in the queue
                        eventVar.wait(lock, [=] { return stopping || !tasks.empty(); });
                        // if its stoping and there are no more tasks we can get out of the thread
                        if (stopping && tasks.empty())
                            break;

                        //again not sure what is the move, probably calling the varible
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    // executing the called function
                    task(socket);
                }
            });
        }
    }
    // tell the program not to throw an exception and if it does just let the program crash
    void stop()
    {
        {
            std::unique_lock<std::mutex> lock{eventMutex};
            stopping = true;
        }

        eventVar.notify_all();
        for (auto &thread : threads)
            //stop each thread from executitaion
            thread.join();
    }
};

#endif
