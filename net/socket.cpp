#include <sys/socket.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bitset>
#include <vector>

#include "socket.h"

using namespace std;
using namespace net;

#define PORT 8080

Socket::Socket()
{

    server_fd = (int)(socket(AF_INET, SOCK_STREAM, 0));
    if (server_fd == 0)
        throw runtime_error("cannot create socket");
}

void Socket::sendStringViaSocket(string text)
{
    char *cstr = new char[text.length() + 1];
    strcpy(cstr, text.c_str());
    int stringSize = strlen(cstr);
    // cout << m_socket << " becfore sending the socket" << endl;
    int byte = write(m_socket, cstr, stringSize);
    if (byte == -1)
    {
        //TODO: add it to the log
        cout << "sending failed" << endl;
    }
}

/*sending a string to all of the connected sockets */
void Socket::sendStringViaSockets(string text)
{

    char *cstr = new char[text.length() + 1];
    strcpy(cstr, text.c_str());
    int stringSize = strlen(cstr);
    cout << acceptedSocketVecotr[0] << endl;
    for (auto &socket : acceptedSocketVecotr)
    {
        int byte = write(socket, cstr, stringSize);
    };
}

void Socket::maintainConnection()
{
    cout << "entering maintainConnection" << endl;
    int listingSocket = server_fd;
    //creating the select set
    fd_set master;
    // clearing the select set
    FD_ZERO(&master);

    // Add our first socket that we're interested in interacting with; the listening socket!
    // It's important that this socket is added for our server or else we won't 'hear' incoming
    // connections
    for (int i = 0; i < acceptedSocketVecotr.capacity(); i++)
    {
        FD_SET(acceptedSocketVecotr[i], &master);
    }
    FD_SET(m_socket, &master);
    // cout << "is the socket in the set? " << FD_ISSET(m_socket, &master) << endl;
    while (true)
    {
        // everytime we call select we are destroying the set. therefor we are creating a copy of it
        fd_set copy = master;
        // creating a list with all of the connected sockets
        int socketCount = select(FD_SETSIZE, &copy, nullptr, nullptr, nullptr);

        for (int i = 0; i < getVector().capacity(); i++)
        {
            int acceptSocket = getVector()[i];
            if (FD_ISSET(acceptSocket, &copy) != 0)
            {
                if (i == server_fd)
                {
                    cout << "a new connection is waiting" << endl;
                    // add the new accepted socket to the FD_SET
                }
                else
                {
                    if (acceptSocket == m_socket)
                    {
                        cout << "The current socket is in the vector list" << endl;
                    }
                    else
                    {
                        cout << acceptSocket << "msocket: " << m_socket << endl;
                    }
                    long valread;
                    // create a function read into string
                    char buffer[30000] = {0};
                    valread = read(acceptSocket, buffer, 30000);
                    string req = buffer;
                    cout << "the requset from the select: " << req << endl;
                }
            }
        }
    }
}

void Socket::Bind()
{
    m_addrlen = sizeof(m_address);

    // choose protocol
    m_address.sin_family = AF_INET;
    //choose interface to bind to(currently binding to all interfaces)
    m_address.sin_addr.s_addr = INADDR_ANY;
    //choose port
    m_address.sin_port = htons(PORT);

    memset(m_address.sin_zero, '\0', sizeof m_address.sin_zero);
    if (bind(server_fd, (struct sockaddr *)&m_address, sizeof(m_address)) < 0)
        throw runtime_error("bind failed");
}

void Socket::Accept()
{
    int sock_accept;
    sock_accept = accept(server_fd, (struct sockaddr *)&m_address, (socklen_t *)&m_addrlen);
    if (sock_accept < 0)
        runtime_error("cannot accept on socket");

    m_socket = sock_accept;
    cout << m_socket << "the current m socket" << endl;
    acceptedSocketVecotr.emplace_back(m_socket);
}

void Socket::Listen()
{
    int err = listen(server_fd, 10);
    cout << " in listing?" << endl;
    if (err < 0)
        throw std::runtime_error("in listen");
}

void Socket::Close()
{
    if (m_socket != 0)
        close(m_socket);
    acceptedSocketVecotr.pop_back();
    cout << "closing connection" << endl;
    m_socket = 0;
}

string Socket::bufferToString()
{
    long valread;
    // create a function read into string
    char buffer[30000] = {0};
    valread = read(m_socket, buffer, 30000);
    string req = buffer;
    return req;
}
