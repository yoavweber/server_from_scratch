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
#include "Threadpool.h"
#include "net/socket.h"
#include "Parser.h"
#include "./http/routes.h"
// #include "WebSocket.h"

using namespace std;
using namespace net;
using namespace route;

#define PORT 8080

void checkHttpType(string position, Socket socket);
void httpRes(ifstream &file, string res);
void acceptConnection(Socket socket);

int Num_Threads = thread::hardware_concurrency();

// move it to the main scope and intilize it with a constructor

Socket socketClass;
int main(int argc, char const *argv[])
{
    //not sure why it is there
    auto start = std::chrono::high_resolution_clock::now();

    Socket socket;
    socket.Bind();
    socket.Listen();

    while (true)
    {
        socket.Accept();
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");

        // // ThreadPool pool{Num_Threads - 1, new_socket};
        // // pool.enqueuq(&acceptConnection);
        acceptConnection(socket);
        socket.Close();
    }
    printf("------------------Closing connection-------------------\n");

    return 0;
}

void checkHttpType(string position, Socket socket)
// return the string and handle sending the information outside of the socket
{
    // should move to parser

    auto httpMethodPosition = position.find(" ");
    auto tests = position.find(" ", httpMethodPosition + 1);
    auto isWebsocketStart = position.find("Upgrade: ");
    auto isWebsocketend = position.find(" ", isWebsocketStart);

    // lower case the filepath
    string filePath = position.substr(httpMethodPosition + 2, tests - 5); // not sure why this is finding two more char after the space
    string httpMethod = position.substr(0, httpMethodPosition);
    if (httpMethod == "GET")
    {
        RoutesHandler routes;
        ifstream inFile;
        routes.sendStaticFile(filePath, socket);
        // if (filePath.size() == 0)
        // {
        //     inFile.open("static/index.html");
        // }
        // // else if (filePath == "chat")
        // // {

        // //     WebSocket webSocket;
        // //     if (webSocket.validWebSocketConnection())
        // //     {
        // //         cout << "open a websocket here!" << endl;

        // //         string webSocketResponse = webSocket.sendHandShake();
        // //         // sending http page
        // //         sendStringViaSocket(socket, webSocketResponse);

        // //         inFile.is_open();
        // //         string successResponse = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
        // //         httpRes(inFile, successResponse, socket);
        // //         sendStringViaSocket(socket, "webSocketResponse");
        // //         testing = true;

        // //         while (true)
        // //         {
        // //             string req = bufferToString(socket);
        // //             // cout << req << endl;
        // //             if (req != "")
        // //             {
        // //                 cout << req << endl;
        // //             }
        // //         }
        // //     }
        // // }
        // else
        // {
        //     inFile.open("static/" + filePath + ".html");
        // }

        // if (inFile.is_open())
        // {
        //     string successResponse = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
        //     httpRes(inFile, successResponse);
        //     cout << "sending success?" << endl;
        // }
        // else
        // {
        //     inFile.open("static/404.html");
        //     string Response404 = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";
        //     cout << "sending failed?" << endl;

        //     httpRes(inFile, Response404);
        // }
    }
}
// create websocket resposne
// HTTP response for GET
// void httpRes(ifstream &file, string res)
// {
//     int size = file.tellg();
//     string content((istreambuf_iterator<char>(file)),
//                    (istreambuf_iterator<char>()));
//     int contentLengthInt = content.size();
//     string contentLength = to_string(contentLengthInt);
//     string response = res + contentLength + "\n\n";
//     response.append(content);

//     socketClass.sendStringViaSocket(response);

//     file.close();
// }

// think where this should go
void acceptConnection(Socket socket)
{

    string req = socket.bufferToString();
    checkHttpType(req, socket);

    printf("------------------HTTP responed has be sent-------------------\n");
    // close(socket);
}
