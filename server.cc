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
#include <queue>
#include <condition_variable>
#include "Threadpool.h"
#include "net/socket.h"
#include "parser.h"
#include "./http/routes.h"
#include "webSocket.h"
#include <bitset>
#include <unistd.h>

using namespace std;
using namespace net;
using namespace route;
using namespace parser;
using namespace websocket;

void checkHttpType(string req, Socket socket);
void acceptConnection(Socket socket);

int Num_Threads = thread::hardware_concurrency();

//change this when refactoring
int main(int argc, char const *argv[])
{
    //not sure why it is there
    auto start = std::chrono::high_resolution_clock::now();

    Socket socket;
    socket.Bind();
    socket.Listen();

    //adding the listing socket to the set

    printf("\n+++++++ Waiting for new connection ++++++++\n\n");

    while (true)
    {

        socket.Accept();
        // ThreadPool pool{Num_Threads - 1, new_socket};
        // pool.enqueuq(&acceptConnection(socket));
        acceptConnection(socket);
    }
    printf("------------------Closing connection-------------------\n");

    return 0;
}

void acceptConnection(Socket socket)
{

    printf("------------------waiting for new requset-------------------\n");
    string req = socket.bufferToString();
    // printf("------------------Recived Response-------------------\n");
    RoutesHandler routes;
    routes.routeRequest(req, socket);

    printf("------------------requset has been handled-------------------\n");
}
