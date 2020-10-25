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
#include "Parser.h"
//#include "WebSocket.h"

using namespace std;

#define PORT 8080

void checkHttpType(string position, int socket);
void httpRes(ifstream &file, string res, int socket);
void acceptConnection(int socket);

int Num_Threads = thread::hardware_concurrency();

int main(int argc, char const *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

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
    // prepring the accept connection from socket, would que 100 before further requts would be refuesd
    if (listen(server_fd, 100) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (true)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        // ThreadPool pool{Num_Threads - 1, new_socket};
        // pool.enqueuq(&acceptConnection);
        acceptConnection(new_socket);
    }
    printf("------------------Closing connection-------------------\n");

    return 0;
}

void checkHttpType(string position, int socket)
// return the string and handle sending the information outside of the socket
{
    cout << position << endl;
    auto httpMethodPosition = position.find(" ");
    auto tests = position.find(" ", httpMethodPosition + 1);
    auto isWebsocketStart = position.find("Upgrade: ");
    auto isWebsocketend = position.find(" ", isWebsocketStart);

    // WebSocket testWeb{position};

    // if (webSocket.validWebSocketConnection())
    // {
    // }

    HttpParser parser{position};
    // lower case the filepath
    string filePath = position.substr(httpMethodPosition + 2, tests - 5); // not sure why this is finding two more char after the space
    string httpMethod = position.substr(0, httpMethodPosition);

    if (httpMethod == "GET")
    {

        ifstream inFile;
        if (filePath.size() == 0)
        {
            inFile.open("static/index.html");
        }
        else if (filePath == "chat")
        {

            HttpParser webSocket{position};
            if (webSocket.validWebSocketConnection())
            {
                cout << "open a websocket here!" << endl;

                string test = webSocket.sendResponse();
                char *cstr = new char[test.length() + 1];
                strcpy(cstr, test.c_str());

                send(socket, cstr, test.size(), 1);
                char buffer[30000] = {0};

                long valread = read(socket, buffer, 30000);
                string req = buffer;

                cout << req << "second request" << endl;

                // while (true)
                // {
                //     char buffer[30000] = {0};

                //     long valread = read(socket, buffer, 30000);
                //     string req = buffer;
                // }
            }
        }
        else
        {
            inFile.open("static/" + filePath + ".html");
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
            inFile.open("static/404.html");
            string Response404 = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";

            httpRes(inFile, Response404, socket);
        }
    }
}
// create websocket resposne
// HTTP response for GET
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

void acceptConnection(int socket)
{
    long valread;
    //// not why I assign this number
    char buffer[30000] = {0};

    valread = read(socket, buffer, 30000);
    string req = buffer;

    checkHttpType(req, socket);

    printf("------------------HTTP responed has be sent-------------------\n");
    close(socket);
}
