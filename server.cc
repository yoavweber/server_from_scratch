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
void httpRes(ifstream &file, string res);
void acceptConnection(Socket socket);

int Num_Threads = thread::hardware_concurrency();

//change this when refactoring
int counter;
int main(int argc, char const *argv[])
{
    //not sure why it is there
    auto start = std::chrono::high_resolution_clock::now();

    Socket socket;
    socket.Bind();
    socket.Listen();
    printf("\n+++++++ Waiting for new connection ++++++++\n\n");

    while (true)
    {

        socket.Accept();
        // ThreadPool pool{Num_Threads - 1, new_socket};
        // pool.enqueuq(&acceptConnection(socket));
        acceptConnection(socket);

        // socket.Close();
    }
    printf("------------------Closing connection-------------------\n");

    return 0;
}

// void checkHttpType(string req, Socket socket)
// // return the string and handle sending the information outside of the socket
// {

//     HttpParser parser{req};
//     RoutesHandler routes;

//     cout << req << endl;
//     string httpMethod = parser.getHeader();
//     string filePath = parser.getPath();

//     if (httpMethod == "GET")
//     {
//         ifstream inFile;
//         if (filePath == "webSocket")
//         {
//             counter += 1;
//             WebSocket webSocketInstense{req};

//             if (webSocketInstense.validWebSocketConnection())
//             {
//                 cout << "the number of connections: " << socket.getConnectionNumber() << endl;

//                 cout << "the websocket connection is valid" << endl;
//                 string webSocketResponse = webSocketInstense.getHandShake();
//                 socket.sendStringViaSocket(webSocketResponse);

//                 // must send 200 after handshake, not sure why I need contet type(204 is not working)
//                 string successResponse = "HTTP/1.1 200 OK\n\n";
//                 socket.sendStringViaSocket(successResponse);

//                 if (counter >= 2)
//                 {

//                     while (true)
//                     {

//                         string clientMessageCrypt = socket.bufferToString();
//                         dataFrame clientFrame = webSocketInstense.decodeFrame(clientMessageCrypt);
//                         if (clientFrame.opcode == 8)
//                         {
//                             cout << "closing connection" << endl;
//                             socket.Close();
//                             break;
//                         }
//                         string clientMessage = clientFrame.payload;
//                         string test = webSocketInstense.encodeFrame(clientMessage);
//                         socket.sendStringViaSockets(test);
//                         if (clientMessage != "")
//                         {

//                             cout << clientFrame.payload << " message payload" << endl;
//                         }
//                     }
//                 }
//                 else
//                 {
//                     string waitingMessage = webSocketInstense.encodeFrame("waiting for a second client");
//                     socket.sendStringViaSocket(waitingMessage);
//                     cout << "counter number " << counter << endl;
//                 }
//                 //     cout << "-----------closing connection------------" << endl;
//             }
//         }
//         else
//         {
//             routes.sendStaticFile(filePath, socket);
//         }
//     }
// }
// create websocket resposne

// think where this should go
void acceptConnection(Socket socket)
{

    printf("------------------waiting for new requset-------------------\n");
    string req = socket.bufferToString();
    printf("------------------Recived Response-------------------\n");
    RoutesHandler routes;
    routes.routeRequest(req, socket);
    // checkHttpType(req, socket);

    printf("------------------requset has been handled-------------------\n");
    // close(socket);
}
