#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <map>
//#include "../net/socket.h"

#include "routes.h"
#include "responses.h"

using namespace route;
using namespace http;
using namespace net;
using namespace std;

RoutesHandler::RoutesHandler()
    : routes{{"yes", "static/path/yes.html"}, {"index", "static/index.html"}, {"chat", "static/path/chat.html"}, {"pdf", "static/path/Computing.pdf"}}
{
}

void RoutesHandler::sendStaticFile(string filePath, Socket socket)
{
    HttpResponses handleResponse;
    if (filePath.size() == 0)
    {
        string route = routes["index"];
        string test = handleResponse.successResponse(route);
        socket.sendStringViaSocket(handleResponse.successResponse(route));
    }
    else if (routes[filePath] == "")
    {
        socket.sendStringViaSocket(handleResponse.response404());
    }
    else
    {
        string route = routes[filePath];
        socket.sendStringViaSocket(handleResponse.successResponse(route));
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