#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include "base64.cpp"
#include <bits/stdc++.h>
#include <openssl/sha.h>
#include "parser.h"
#include "webSocket.h"
#include "net/socket.h"
#include <poll.h>

#define WEBSOCKETHASH "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

using namespace parser;
using namespace websocket;
using namespace std;
using namespace net;

void WebSocket::handleWebSocketConnection(int socket)
{
    long valread;
    //// not why I assign this number
    char buffer[30000] = {0};

    valread = read(socket, buffer, 30000);
    string req = buffer;

    bool isValid = validWebSocketConnection();

    //close(socket);
}

bool WebSocket::validWebSocketConnection()
{
    // check http version
    // create a function that recive a string and give the index after the space
    string websocket = getReqLineValue("Upgrade: ");
    string upgrade = getReqLineValue("Connection: ");

    //TODO: change this after refactoring the parser
    if (websocket == "websocket" && upgrade == "Upgrade" || upgrade == "keep-alive, Upgrade")
    {
        return true;
    }
    // add a log that explains why the connection is failing
    return false;
}

string WebSocket::getHandShake()
{
    string key = getWebsocketAcceptKey();
    // the c in the end is important for some reason
    string response = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + key + "\r\nc\r\n";
    // std::string response =
    //     "HTTP/1.1 101 Switching Protocols\r\n"
    //     "Upgrade: websocket\r\n"
    //     "Connection: Upgrade\r\n"
    //     "Sec-WebSocket-Accept: " +
    //     key + "\r\n"
    //           "\r\n";

    return response;
}

// TODO: decide of the maintainConnection function is going outside
int WebSocket::handleHandShake(Socket socket)
{
    if (validWebSocketConnection())
    {
        cout << "the number of connections: " << socket.getConnectionNumber() << endl;

        cout << "the websocket connection is valid" << endl;
        string webSocketResponse = getHandShake();
        socket.sendStringViaSocket(webSocketResponse);

        // must send 200 after handshake, not sure why I need contet type(204 is not working)
        string successResponse = "HTTP/1.1 200 OK\n\n";
        socket.sendStringViaSocket(successResponse);
        maintainConnection(socket);
        return 0;
    }
    else
    {

        return -1;
    }
}

void WebSocket::maintainConnection(Socket socket)
{
    if (socket.getConnectionNumber() > 3)
    {
        cout << "------------- entering while -----------" << endl;

        // looping over the sockets
        //creating the select set
        fd_set master;
        // clearing the select set
        FD_ZERO(&master);

        vector<int> acceptedSocketVector = socket.getVector();

        // vector<int> acceptedSocketVector;
        // acceptedSocketVector.push_back(8);
        // acceptedSocketVector.push_back(10);
        int vectorSize = acceptedSocketVector.capacity();

        struct pollfd pfds[vectorSize];

        // Add our first socket that we're interested in interacting with; the listening socket!
        // It's important that this socket is added for our server or else we won't 'hear' incoming
        // connections
        for (int i = 0; i < vectorSize; i++)
        {
            FD_SET(acceptedSocketVector[i], &master);
            pfds[i].fd = acceptedSocketVector[i];
            pfds[i].events = POLLOUT;
        }

        while (true)
        {
            // everytime we call select we are destroying the set. therefor we are creating a copy of it
            fd_set read_fds = master;
            fd_set write_fds = master;
            // creating a list with all of the connected sockets
            int socketCount = select(11, &read_fds, &write_fds, nullptr, nullptr);
            // int socketCount = select(FD_SETSIZE, &read_fds, nullptr, nullptr, nullptr);

            int test = socket.getSocket();
            string t;
            // int num_events = poll(pfds, vectorSize, -1);
            for (int i = 0; i < vectorSize; i++)
            {
                int acceptSocket = acceptedSocketVector[i];
                if ((FD_ISSET(acceptSocket, &read_fds) != 0))
                {

                    string clientMessageCrypt = socket.bufferToString(acceptSocket);
                    cout << "recived message from socket: " << acceptSocket << endl;
                    dataFrame clientFrame = decodeFrame(clientMessageCrypt);
                    if (clientFrame.opcode == 8)
                    {
                        cout << "closing connection" << endl;
                        socket.Close();
                        break;
                    }
                    string clientMessage = clientFrame.payload;

                    t = encodeFrame(clientMessage);

                    // for (int i = 0; i < vectorSize; i++)
                    // {
                    //     int outSock = acceptedSocketVector[i];
                    //     if (outSock != acceptSocket)
                    //     {
                    //         cout << outSock << endl;
                    //         socket.sendStringViaSocket(t, 8);
                    //     }
                    // }

                    for (int i = 0; i < vectorSize; i++)
                    {
                        int outSock = acceptedSocketVector[i];
                        if ((FD_ISSET(outSock, &write_fds) != 0) && outSock != acceptSocket)
                        {
                            cout << "the socket that would get the message: " << outSock << endl;
                            socket.sendStringViaSocket(t, outSock);
                        }
                    }

                    // // printf("Write FD: %d\n", FD_ISSET(acceptSocket, &write_fds));

                    //---------------------trying to go with polling -----------
                    // for (int j = 0; j < vectorSize; j++)
                    // {
                    //     int sendSocket = acceptedSocketVector[j];
                    //     if (pfds[j].revents & POLLOUT && pfds[j].fd != acceptSocket)
                    //     {
                    //         socket.sendStringViaSocket(t, sendSocket);
                    //     }
                    //     else
                    //     {
                    //         cout << "damn..." << pfds[j].fd << endl;
                    //     }
                    // }
                    if (clientMessage != "")
                    {

                        cout << "message payload: " << clientFrame.payload << endl;
                    }
                }
            }
            // socket.sendStringViaSockets(t);
            // socket.maintainConnection();
            // string clientMessageCrypt = socket.bufferToString();
            // dataFrame clientFrame = decodeFrame(clientMessageCrypt);
            // if (clientFrame.opcode == 8)
            // {
            //     cout << "closing connection" << endl;
            //     socket.Close();
            //     break;
            // }
            // string clientMessage = clientFrame.payload;
            // string test = encodeFrame(clientMessage);
            // socket.sendStringViaSockets(test);
            // if (clientMessage != "")
            // {

            //     cout << "message payload: " << clientFrame.payload << endl;
            // }
        }
    }
    else
    {
        string waitingMessage = encodeFrame("waiting for a second client");
        socket.sendStringViaSocket(waitingMessage);
    }
}

// string WebSocket::handleWebsocketMessage(string req)
// {
//     dataFrame clientFrame = decodeFrame(req);
//     if (clientFrame.opcode == 8)
//     {
//         cout << "closing connection" << endl;
//         return "close";
//     }
//     string clientMessage = clientFrame.payload;
//     string test = encodeFrame(clientMessage);
//     if (clientMessage != "")
//     {
//         cout << "message payload: " << clientFrame.payload << endl;
//     }
//     return test;
// }

bool WebSocket::handleWebsocketMessage(string req, Socket socket)
{
    dataFrame clientFrame = decodeFrame(req);
    if (clientFrame.opcode == 8)
    {
        cout << "closing connection" << endl;
        socket.Close();
        return false;
    }
    string clientMessage = clientFrame.payload;
    string test = encodeFrame(clientMessage);
    socket.sendStringViaSockets(test);
    if (clientMessage != "")
    {
        cout << "message payload: " << clientFrame.payload << endl;
    }
    return true;
}

dataFrame WebSocket::decodeFrame(string rawData)
{
    dataFrame data;

    // if fin is true, this is the last massege
    data.fin = rawData[0] & 0b1000'0000;
    // the data must be 1, if its not then stop the connection
    data.reserved = rawData[0] & 0b0111'0000;
    // define the type of data
    data.opcode = rawData[0] & 0b0000'1111;
    // define if the payload data is mask(if the client send it it must be 1)
    data.mask = rawData[1] & 0b1000'0000;
    // the length of the payload data.
    data.payload_size = rawData[1] & 0b0111'1111;

    // counting the bytes until the payload
    int byteCount = 2;

    if (data.payload_size == 126)
    {
        data.payload_size += rawData[2] + rawData[3];
        byteCount += 2;
    }
    else if (data.payload_size == 127)
    {
        for (int i = 0; i < 8; i++)
        {
            data.payload_size += rawData[i + 2];
        }
        byteCount += 8;
    }
    else
    {
        data.payload = data.payload_size;
    }

    if (data.mask)
    {
        byteCount += 4;

        for (int i = 0; i < 4; i++)
        {
            data.mask_key[i] = rawData[i + 2];
        }
    }
    rawData.erase(0, byteCount);
    data.payload = move(rawData);

    int i;
    for (auto &c : data.payload)
    {
        c = c ^ data.mask_key[i++ % 4];
    };

    return data;
}

//currently sends only one message(fin is true)
//currently the payload would always be 1
string WebSocket::encodeFrame(string data)
{
    string frameData;

    // move to different function

    int dataSize = data.size() | 0b0000'0000;

    //if(dataSize > 125)

    frameData = 0b1000'0001;

    frameData += dataSize;
    // adding the payload info to the string
    for (char &_char : data)
    {
        frameData += _char | 0b0000'0000;
    }

    // for (int i = 0; i < frameData.length(); i++)
    // {
    //     auto t = bitset<8>(frameData[i]);
    //     cout << t << endl;
    // }

    return frameData;
}

string WebSocket::getWebsocketAcceptKey()
{
    string webSocketKey = getReqLineValue("Sec-WebSocket-Key: ");
    string keyAndHash = webSocketKey + WEBSOCKETHASH;
    string hexHash = sha1(keyAndHash);
    vector<base64::byte> data(begin(hexHash), end(hexHash));
    auto encoded = base64::encode(data);
    return encoded;
}

string WebSocket::sha1(const string &input)
// string WebSocket::sha1(const string &input, size_t iterations = 1)
{
    string hash;

    hash.resize(160 / 8);
    SHA1(reinterpret_cast<const unsigned char *>(&input[0]), input.size(), reinterpret_cast<unsigned char *>(&hash[0]));

    for (size_t c = 1; c < 1; ++c)
        SHA1(reinterpret_cast<const unsigned char *>(&hash[0]), hash.size(), reinterpret_cast<unsigned char *>(&hash[0]));

    return hash;
}