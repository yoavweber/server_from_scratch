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

using namespace std;

#define PORT 8080
#define THREAD_POOL_SIZE 20
typedef void (*Connection)(int);

//https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean

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
    void enqueuq(Connection task)
    {
        // why are we creating another scope?
        {
            unique_lock<mutex> lock{eventMutex};
            // insert element to the queue, not sure why using the move
            tasks.emplace(move(task));
        }
        // notifying one of the threads to wake up and take the job
        eventVar.notify_one();
    }

private:
    vector<thread> threads;

    condition_variable eventVar;

    mutex eventMutex;

    bool stopping = false;
    // hold the list of task to be executed on the thread pool
    queue<Connection> tasks;
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
                        // what is uniuq lock?
                        unique_lock<mutex> lock{eventMutex};
                        // the event varible would wake up if we are stopping or if there is task in the queue
                        eventVar.wait(lock, [=] { return stopping || !tasks.empty(); });
                        // if its stoping and there are no more tasks we can get out of the thread
                        if (stopping && tasks.empty())
                            break;

                        //again not sure what is the move, probably calling the varible
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    // not sure what is executing here
                    cout << "how many times?" << endl;
                    task(socket);
                }
            });
        }
    }
    // tell the program not to throw an exception and if it does just let the program crash
    void stop()
    {
        {
            unique_lock<mutex> lock{eventMutex};
            stopping = true;
        }

        eventVar.notify_all();
        for (auto &thread : threads)
            //stop each thread from executitaion
            thread.join();
    }
};

void checkHttpType(string position, int socket);
void httpRes(ifstream &file, string res, int socket);
void handleConnection(int socket);

int Num_Threads = thread::hardware_concurrency();

int main(int argc, char const *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    cout << Num_Threads << endl;

    int server_fd, new_socket;

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    // choose protocol
    address.sin_family = AF_INET;
    //choose interface to bind to(currently binding to all interfaces)
    address.sin_addr.s_addr = INADDR_ANY;
    //choose port
    address.sin_port = htons(PORT);

    //no sure whats going in here
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    //listening to the created socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    // prepring the accept connection from socket, would que 10 before further requts would be refuesd
    if (listen(server_fd, 100) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        // handleConnection(new_socket);
        // multithread without pool
        // thread t(&handleConnection, new_socket);
        // t.detach();

        ThreadPool pool{4, new_socket};
        pool.enqueuq(&handleConnection);

        // for (auto i = 0; i < 4; i++)
        // {
        //     cout << "Creating new connections?" << endl;
        // }
    }
    printf("------------------Closing connection-------------------\n");

    return 0;
}

void checkHttpType(string position, int socket)
// return the string and handle sending the information outside of the socket
{
    auto httpMethodPosition = position.find(" ");
    auto tests = position.find(" ", httpMethodPosition + 1);
    string filePath = position.substr(httpMethodPosition + 2, tests - 5); // not sure why this is finding two more char after the space
    string httpMethod = position.substr(0, httpMethodPosition);
    if (httpMethod == "GET")
    {
        ifstream inFile;

        cout << filePath + ".html" << endl;
        if (filePath.size() == 0)
        {
            inFile.open("index.html");
        }
        else
        {
            inFile.open(filePath + ".html");
        }
        inFile.seekg(0, inFile.end);
        int size = inFile.tellg();
        inFile.seekg(0, inFile.beg);

        if (inFile.is_open())
        {
            string successResponse = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
            httpRes(inFile, successResponse, socket);
        }
        else
        {
            inFile.open("404.html");
            string Response404 = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";

            httpRes(inFile, Response404, socket);
        }
    }
}

void httpRes(ifstream &file, string res, int socket)
{
    int size = file.tellg();
    string content((istreambuf_iterator<char>(file)),
                   (istreambuf_iterator<char>()));
    int contentLengthInt = content.size();
    string contentLength = to_string(contentLengthInt);
    string Response404 = res + contentLength + "\n\n";
    Response404.append(content);

    char *cstr = new char[Response404.length() + 1];
    strcpy(cstr, Response404.c_str());

    send(socket, cstr, size + 1000, 1);
    file.close();
}

void handleConnection(int socket)
{
    long valread;
    char buffer[30000] = {0};
    // not sure what is it used
    valread = read(socket, buffer, 30000);
    string req = buffer;

    checkHttpType(req, socket);

    printf("------------------Hello message sent-------------------\n");
    close(socket);
}