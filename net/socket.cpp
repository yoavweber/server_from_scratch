#include <sys/socket.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bitset>

#include "socket.h"

using namespace std;
using namespace net;

#define PORT 8080

Socket::Socket()
{
    // if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    // {
    //     perror("In socket");
    //     exit(EXIT_FAILURE);
    // }
    server_fd = (int)(socket(AF_INET, SOCK_STREAM, 0));
    if (server_fd == 0)
        throw runtime_error("cannot create socket");
}

void Socket::sendStringViaSocket(string text)
{
    char *cstr = new char[text.length() + 1];
    strcpy(cstr, text.c_str());
    int stringSize = strlen(cstr);
    // for (int i = 0; i < text.length(); i++)
    // {
    //     auto t = bitset<8>(text[i]);
    //     cout << t;
    // }
    int byte = write(m_socket, cstr, stringSize);
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