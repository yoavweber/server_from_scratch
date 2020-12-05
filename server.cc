#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "net/Threadpool.h"
#include "net/socket.h"
#include "./http/routes.h"

using namespace std;
using namespace net;
using namespace route;

void checkHttpType(string req, Socket socket);
void acceptConnection(Socket socket);

int Num_Threads = thread::hardware_concurrency();

//change this when refactoring
int main(int argc, char const *argv[])
{

    Socket socket;
    socket.Bind();
    socket.Listen();

    //TODO:adding the listing socket to the socket vector
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
    RoutesHandler routes;
    routes.routeRequest(req, socket);

    printf("------------------requset has been handled-------------------\n");
}
