#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <vector>

#ifndef SOCKET_H // include guard
#define SOCKET_H

// add the whole processs of creating a socket
namespace net
{
    class Socket
    {
        int server_fd, m_socket, m_addrlen;
        std::vector<int> acceptedSocketVector;
        //int m_port;
        struct sockaddr_in m_address;

    public:
        Socket();
        // change buffer to string to read
        std::string bufferToString();
        std::string bufferToString(int socket);

        int getSocket()
        {
            return m_socket;
        }

        int getConnectionNumber()
        {
            return acceptedSocketVector.capacity();
        }

        //     // add the port number

        void Bind();
        void Listen();
        void Accept();
        void Close();
        void sendStringViaSocket(std::string text);
        void sendStringViaSocket(std::string text, int socket);
        void sendStringViaSockets(std::string text);

        void maintainConnection();
    };

} // namespace net

#endif