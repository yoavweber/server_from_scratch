#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
// #include "util.cpp"
// #include "base64.cpp"
// #include <bits/stdc++.h>
// #include <openssl/sha.h>

#ifndef WEBSOCKET_H // include guard
#define WEBSOCKET_H

#include "parser.h"

struct dataFrame
{
    bool fin = true;
    // using
    uint8_t mask_key[4] = {};
    bool mask = false;
    uint8_t opcode = 0x1;
    uint8_t reserved = 0;
    uint64_t payload_size = 0;
    std::string payload = {};
};

namespace websocket
{
    class WebSocket : public parser::HttpParser
    {
    public:
        WebSocket(std::string req) : parser::HttpParser(req)
        {
            req = req;
        };
        // WebSocket(std::string req) : HttpParser(req);
        void handleWebSocketConnection(int socket);
        // {
        //     long valread;
        //     //// not why I assign this number
        //     char buffer[30000] = {0};

        //     valread = read(socket, buffer, 30000);
        //     std::string req = buffer;

        //     bool isValid = validWebSocketConnection();
        //     // if (isValid)
        //     // {
        //     //     // open connection
        //     // }

        //     //close(socket);
        // }
        dataFrame decodeFrame(std::string rawData);

        bool validWebSocketConnection();
        // {
        //     // check http version
        //     // create a function that recive a string and give the index after the space
        //     std::string websocket = getReqLineValue("Upgrade: ");
        //     // std::string websocket = getReqLineValue("Upgrade: ");
        //     std::string upgrade = getReqLineValue("Connection: ");

        //     if (websocket == "websocket" && upgrade == "Upgrade")
        //     {
        //         return true;
        //     }
        //     // add a log that explains why the connection is failing
        //     return false;
        // }

        std::string getHandShake();
        // {
        //     std::string key = getWebsocketAcceptKey();
        //     std::string response = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + key + "\r\nc\r\n";
        //     // std::string response =
        //     //     "HTTP/1.1 101 Switching Protocols\r\n"
        //     //     "Upgrade: websocket\r\n"
        //     //     "Connection: Upgrade\r\n"
        //     //     "Sec-WebSocket-Accept: " +
        //     //     key + "\r\n"
        //     //           "\r\n";

        //     return response;
        // }

    private:
        std::string req;

        std::string getWebsocketAcceptKey();
        // {
        //     std::string webSocketKey = getReqLineValue("Sec-WebSocket-Key: ");
        //     std::string keyAndHash = webSocketKey + WEBSOCKETHASH;
        //     std::string hexHash = sha1(keyAndHash);
        //     std::vector<base64::byte> data(begin(hexHash), end(hexHash));
        //     auto encoded = base64::encode(data);
        //     return encoded;
        // }

        static std::string sha1(const std::string &input);
        // static std::string sha1(const std::string &input, std::size_t iterations);

        // {
        //     std::string hash;

        //     hash.resize(160 / 8);
        //     SHA1(reinterpret_cast<const unsigned char *>(&input[0]), input.size(), reinterpret_cast<unsigned char *>(&hash[0]));

        //     for (std::size_t c = 1; c < iterations; ++c)
        //         SHA1(reinterpret_cast<const unsigned char *>(&hash[0]), hash.size(), reinterpret_cast<unsigned char *>(&hash[0]));

        //     return hash;
        // }
    };
} // namespace websocket
#endif