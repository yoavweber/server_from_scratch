#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>

#ifndef SOCKET // include guard
#define MY_CLASS_H

namespace net
{
    class Socket
    {

    public:
        std::string bufferToString(int socket);
        void sendStringViaSocket(int socket, std::string text);
        // std::string bufferToString(int socket);
        // void sendStringViaSocket(int socket, std::string text);
    };
} // namespace net

#endif