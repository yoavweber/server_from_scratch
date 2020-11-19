#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <map>
#include "../net/socket.h"

#ifndef ROUTES_H // include guard
#define ROUTES_H

// add the whole processs of creating a socket
namespace route
{
    class RoutesHandler
    {
        std::map<std::string, std::string> routes;

    public:
        RoutesHandler();
        void sendStaticFile(std::string filePath, net::Socket socket);

    private:
        bool checkIfRouteExist(std::string string);
        void handleRequst(std::string req);
    };
} // namespace route

#endif