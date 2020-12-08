#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <map>

#include "routes.h"
#include "responses.h"
#include "parser.h"
#include "../net/webSocket.h"

using namespace route;
using namespace http;
using namespace net;
using namespace std;
using namespace parser;

RoutesHandler::RoutesHandler()
    : routes{{"yes", "static/path/yes.html"}, {"index", "static/index.html"}, {"chat", "static/path/websocketClient.html"}, {"pdf", "static/path/Computing.pdf"}}
{
}

void RoutesHandler::routeRequest(string req, Socket socket)
{
    HttpParser parser{req};
    cout << req << endl;
    string httpMethod = parser.getHeader();
    string filePath = parser.getPath();

    if (httpMethod == "GET")
    {

        if (filePath == "webSocket")
        {
            websocket::WebSocket webSocketInstense{req};
            if (webSocketInstense.handleHandShake(socket) == -1)
            {
                //TODO:add this to the log
                cout << "handshake is not valid" << endl;
            }
        }
        else
        {
            sendStaticFile(filePath, socket);
        }
    }
    else
    {
        //TODO:change this to different error
        throw runtime_error("currently the server only handle GET request and not " + httpMethod);
    }
}

void RoutesHandler::sendStaticFile(string filePath, Socket socket)
{
    HttpResponses handleResponse;

    // would make sense to change it to switch experssion
    if (filePath.size() == 0)
    {
        string route = routes["index"];
        socket.sendStringViaSocket(handleResponse.successResponse(route));
    }
    else if (routes[filePath] == "")
    {
        socket.sendStringViaSocket(handleResponse.response404());
    }
    else if (filePath == "chat")
    {
        string route = routes[filePath];
        socket.sendStringViaSocket(handleResponse.successResponse(route));
    }
    else
    {
        string route = routes[filePath];
        socket.sendStringViaSocket(handleResponse.successResponse(route));
        cout << filePath << " route from else" << endl;
    }

    socket.Close();
}

// void RoutesHandler::sendSuc
bool RoutesHandler::checkIfRouteExist(std::string string)
{
    return false;
}
