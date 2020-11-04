#include <sys/socket.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "socket.h"

using namespace std;
using namespace net;

string Socket::bufferToString(int socket)
{
    long valread;
    //// not why I assign this number
    // create a function read into string
    char buffer[30000] = {0};
    valread = read(socket, buffer, 30000);
    string req = buffer;
    return req;
}

void Socket::sendStringViaSocket(int socket, string text)
{
    cout << "response sent" << endl;

    char *cstr = new char[text.length() + 1];
    strcpy(cstr, text.c_str());
    send(socket, cstr, text.size(), 1);
} // HttpParser parser{position};
