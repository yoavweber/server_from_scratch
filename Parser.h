#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <regex>
#include <bits/stdc++.h>
#include <openssl/sha.h>
#include "base64.cpp"

#ifndef HttpParser_H // include guard
#define Parser_H
#define WEBSOCKETHASH "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

class HttpParser
{
public:
    explicit HttpParser(std::string req)
        : req(req){};

    std::string getHeader(std::string req)
    {
        auto httpMethodPosition = req.find(" ");
        auto tests = req.find(" ", httpMethodPosition + 1);
        std::string httpMethod = req.substr(0, httpMethodPosition);
        return httpMethod;
    }

    std::string getPath(std::string req)
    {
        auto httpMethodPosition = req.find(" ");
        auto tests = req.find(" ", httpMethodPosition + 1);
        std::string filePath = req.substr(httpMethodPosition + 2, tests - 5); // not sure why this is finding two more char after the space
        return filePath;
    }

    std::string sendResponse()
    {
        std::string key = getWebsocketAcceptKey();
        std::string response =
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: " +
            key + "\r\n"
                  "\r\n";

        std::cout << response << "!!!!!!!!!!!" << std::endl;

        return response;
    }

    bool validWebSocketConnection()
    {
        // check http version
        // create a function that recive a string and give the index after the space
        std::string websocket = getReqLineValue("Upgrade: ");
        // std::string websocket = getReqLineValue("Upgrade: ");
        std::string upgrade = getReqLineValue("Connection: ");
        // getWebsocketAcceptKey();
        std::cout << websocket << upgrade << std::endl;

        if (websocket == "websocket" && upgrade == "Upgrade")
        {
            return true;
        }
        return false;
    }

protected:
    std::vector<std::string> requestLines;
    std::string req;

    void splitToLines(std::string req)
    {
        // read about stringstream more https://www.geeksforgeeks.org/stringstream-c-applications/
        auto ss = std::stringstream{req};
        std::string line;
        while (std::getline(ss, line, '\n'))
        {
            requestLines.push_back(line);
        }
    }

    std::string getLine(std::string keyWord)
    {
        std::regex r("\s*" + keyWord + "\s*.*"); // entire match will be 2 numbers
        std::smatch m;
        std::regex_search(req, m, r);
        auto v = m[0];
        return v;
    }

    std::string getReqLineValue(std::string keyWord)
    {
        std::string firstWordLine = getLine(keyWord);
        auto wordIndex = firstWordLine.find(" ");
        std::string requiredValue = firstWordLine.substr(wordIndex + 1);
        return requiredValue;
    }

    std::string getWebsocketAcceptKey()
    {
        std::string webSocketKey = getReqLineValue("Sec-WebSocket-Key: ");
        std::string keyAndHash = webSocketKey + WEBSOCKETHASH;
        std::string binaryHash = sha1(keyAndHash);
        std::vector<base64::byte> data(begin(binaryHash), end(binaryHash));
        auto encoded = base64::encode(data);
        //std::cout << encoded << std::endl;
        return encoded;
    }

    static std::string sha1(const std::string &input, std::size_t iterations = 1) noexcept
    {
        std::string hash;

        hash.resize(160 / 8);
        SHA1(reinterpret_cast<const unsigned char *>(&input[0]), input.size(), reinterpret_cast<unsigned char *>(&hash[0]));

        for (std::size_t c = 1; c < iterations; ++c)
            SHA1(reinterpret_cast<const unsigned char *>(&hash[0]), hash.size(), reinterpret_cast<unsigned char *>(&hash[0]));

        return hash;
    }
};

#endif
