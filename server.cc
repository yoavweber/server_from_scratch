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
#include "parser.h"
#include "./http/routes.h"
#include "webSocket.h"
#include <bitset>

using namespace std;
using namespace net;
using namespace route;
using namespace parser;
using namespace websocket;

void checkHttpType(string req, Socket socket);
void httpRes(ifstream &file, string res);
void acceptConnection(Socket socket);

int Num_Threads = thread::hardware_concurrency();

// move it to the main scope and intilize it with a constructor

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

        // // ThreadPool pool{Num_Threads - 1, new_socket};
        // // pool.enqueuq(&acceptConnection);
        acceptConnection(socket);
        socket.Close();
    }
    printf("------------------Closing connection-------------------\n");

    return 0;
}

void checkHttpType(string req, Socket socket)
// return the string and handle sending the information outside of the socket
{

    HttpParser parser{req};
    RoutesHandler routes;

    cout << req << endl;
    string httpMethod = parser.getHeader();
    string filePath = parser.getPath();

    if (httpMethod == "GET")
    {
        ifstream inFile;
        if (filePath == "chat")
        {

            WebSocket webSocketInstense{req};

            if (webSocketInstense.validWebSocketConnection())
            {

                cout << "the websocket connection is valid" << endl;
                string webSocketResponse = webSocketInstense.getHandShake();
                socket.sendStringViaSocket(webSocketResponse);

                // must send 200 after handshake, not sure why I need contet type(204 is not working)
                string successResponse = "HTTP/1.1 200 OK\n\n";
                socket.sendStringViaSocket(successResponse);

                string clientMessage = socket.bufferToString();

                webSocketInstense.decodeFrame(clientMessage);

                socket.sendStringViaSocket(webSocketInstense.encodeFrame("hey"));

                while (true)
                {
                    string clientMessageCrypt = socket.bufferToString();
                    string test = webSocketInstense.encodeFrame("test");
                    socket.sendStringViaSocket(test);
                    dataFrame clientFrame = webSocketInstense.decodeFrame(clientMessageCrypt);
                    if (clientFrame.opcode == 8)
                    {
                        socket.Close();
                        break;
                    }
                    string clientMessage = clientFrame.payload;
                    if (clientMessage != "")
                    {

                        cout << clientMessage << endl;
                    }
                    // socket.sendStringViaSocket(webSocketInstense.encodeFrame(clientMessage));
                    // cout << "closing after sending string?" << endl;
                }
                //     cout << "-----------closing connection------------" << endl;
            }
        }
        else
        {
            routes.sendStaticFile(filePath, socket);
        }
    }
}
// create websocket resposne

// think where this should go
void acceptConnection(Socket socket)
{

    printf("------------------waiting for new requset-------------------\n");
    string req = socket.bufferToString();
    printf("------------------Recived Response-------------------\n");
    checkHttpType(req, socket);

    printf("------------------HTTP responed has been sent-------------------\n");
    // close(socket);
}
