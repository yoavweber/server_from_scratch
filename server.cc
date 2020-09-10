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

using namespace std;

#define PORT 8069
string checkHttpType(string position, int socket);

int main(int argc, char const *argv[])
{

    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

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
        string res = buffer;
        checkHttpType(res, new_socket);
        cout << res << endl;
        // printf("%s\n",test );
        // printf("%s\n",buffer );

        // write(new_socket, hello, strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    printf("------------------Closing connection-------------------\n");

    return 0;
}

string checkHttpType(string position, int socket)
{
    auto httpMethodPosition = position.find(" ");
    string hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1000\n\n";

    string httpMethod = position.substr(0, httpMethodPosition);
    if (httpMethod == "GET")
    {
        // ofstream inFile;
        ifstream inFile;
        inFile.open("index.html");
        inFile.seekg(0, inFile.end);
        int size = inFile.tellg();
        inFile.seekg(0, inFile.beg);

        if (inFile.is_open())
        {

            string content((istreambuf_iterator<char>(inFile)),
                           (istreambuf_iterator<char>()));
            hello.append(content);

            char *cstr = new char[hello.length() + 1];
            strcpy(cstr, hello.c_str());

            send(socket, cstr, size + 1000, 1);

            // while (getline(inFile, line))
            // {
            //     cout << line << '\n';
            // }
            inFile.close();
        }
        // if (!inFile)
        // {
        //     cerr << "Unable to open file datafile.txt";
        //     exit(1); // call system to stop
        // }

        printf("%d\n", size);
        // write(socket, &hello, size);

        // for (int i = 0; i < size; i++)
        // {
        //     write(socket, &hello, 1);
        // }
        // send(socket, hello, strlen(hello), 1);
    }
}
