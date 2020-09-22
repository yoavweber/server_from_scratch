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

using namespace std;

#define PORT 8080
string checkHttpType(string position, int socket);
void httpRes(ifstream &file, string res, int socket);

// create a place with dedicted http respons
// start to look into testing
//finish the regex https://rextester.com/JPSDEQ88022
// https://www.regextester.com/97722
//https://www.rexegg.com/regex-quickstart.html

int main(int argc, char const *argv[])
{

    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    //listening to the created socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        string req = buffer;

        checkHttpType(req, new_socket);
        // cout << req << endl;

        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    printf("------------------Closing connection-------------------\n");

    return 0;
}

string checkHttpType(string position, int socket)
// return the string and handle sending the information outside of the socket
{
    auto httpMethodPosition = position.find(" ");
    auto tests = position.find(" ", httpMethodPosition + 1);
    string filePath = position.substr(httpMethodPosition + 2, tests - 5); // not sure why this is finding two more char after the space
    string httpMethod = position.substr(0, httpMethodPosition);
    if (httpMethod == "GET")
    {
        // ofstream inFile;
        ifstream inFile;

        cout << filePath + ".html" << endl;
        if (filePath.size() == 0)
        {
            inFile.open("index.html");
        }
        else
        {
            inFile.open(filePath + ".html");
        }
        inFile.seekg(0, inFile.end);
        int size = inFile.tellg();
        inFile.seekg(0, inFile.beg);

        // move this to a different function
        if (inFile.is_open())
        {
            string successResponse = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
            httpRes(inFile, successResponse, socket);
        }
        else
        {
            inFile.open("404.html");
            string Response404 = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";

            httpRes(inFile, Response404, socket);
        }
    }
    return "h";
}

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