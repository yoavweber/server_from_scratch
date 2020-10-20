#pragma once

#include "Parser.h"

class WebSocket : public HttpParser
{

public:
    bool validWebSocketConnection()
    {
        // check http version
        // create a function that recive a string and give the index after the space
        auto isWebsocket = requestLines[2].find(" ");
        auto isUpgrade = requestLines[3].find(" ");
        std::string websocket = requestLines[2].substr(isWebsocket);
        std::string upgrade = requestLines[3].substr(isUpgrade);
        getWebsocketAcceptKey();

        if (websocket == "websocket" && upgrade == "Upgrade")
        {
            return true;
        }
        // std::cout << upgrade << std::endl;
        return false;
    }
}
