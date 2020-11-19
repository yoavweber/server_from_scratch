#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>

#ifndef SOCKET_H // include guard
#define SOCKET_H

// add the whole processs of creating a socket
namespace net
{
    class Socket
    {
        int server_fd, m_socket, m_addrlen;
        //int m_port;
        struct sockaddr_in m_address;

    public:
        // explicit Socket(int socket, sockaddr_in address, int port) : m_socket(socket), m_address(address), m_port(port){};
        Socket();
        // change buffer to string to read
        std::string bufferToString(int socket);

        int getSocket()
        {
            return m_socket;
        }
        //     // add the port number

        void Bind();

        void Listen();
        void Accept();
        void Close();
        void sendStringViaSocket(int socket, std::string text);
        // private:
        //     void Bind();
    };

} // namespace net

#endif