#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "net/socket.h"

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
        inline static std::vector<int> webSockets;

    public:
        WebSocket(std::string req) : parser::HttpParser(req)
        {
            req = req;
        };
        void handleWebSocketConnection(int socket);

        int handleHandShake(net::Socket socket);
        dataFrame decodeFrame(std::string rawData);

        std::string encodeFrame(std::string rawData);

        // move it into private
        bool validWebSocketConnection();
        void maintainConnection(net::Socket socket);
        bool handleWebsocketMessage(std::string req, net::Socket socket);

        std::string getHandShake();

        void addSocketToVector(int value)
        {
            webSockets.push_back(value);
        }

    private:
        std::string req;

        std::string getWebsocketAcceptKey();

        static std::string sha1(const std::string &input);
    };
} // namespace websocket
#endif