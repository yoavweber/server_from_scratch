#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <map>

#include "routes.h"

using namespace route;
using namespace std;

RoutesHandler::RoutesHandler()
    : routes{{"yes", "static/path/yes.html"}, {"index", "static/index.html"}, {"chat", "static/path/chat.html"}, {"pdf", "static/path/Computing.pdf"}}
{
}

void RoutesHandler::sendStaticFile(string response, string filePath, int socket)
{
    if (filePath.size() == 0)
    {
        cout << "send the static file" << endl;
    }
    else if (routes[filePath] == "")
    {

        cout << "no route" << endl;
    }
}

bool RoutesHandler::checkIfRouteExist(std::string string)
{
    return false;
}

void RoutesHandler::handleRequst(std::string req)
{
    cout << "test" << endl;
}

// {
//     class RoutesHandler
//     {
//         map<string, string> routes;

//     public:
//         RoutesHandler(map<string, string> routes);
//         void sendStaticFile(string response);
//         void sendStringViaSocket(int socket, string text);

//     private:
//         bool checkIfRouteExist(string string);
//         void handleRequst(string req);
//     };
// } // namespace routes
