#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <regex>
// #include <bits/stdc++.h>
// #include <openssl/sha.h>
#include <bitset>
#include "parser.h"
// #include "base64.cpp"

using namespace std;
using namespace parser;

string HttpParser::getHeader()
{
    auto httpMethodPosition = req.find(" ");
    auto tests = req.find(" ", httpMethodPosition + 1);
    string httpMethod = req.substr(0, httpMethodPosition);
    return httpMethod;
}

string HttpParser::getPath()
{
    auto httpMethodPosition = req.find(" ");
    auto tests = req.find(" ", httpMethodPosition + 1);
    // lower case the filepath(can use tolower())
    string filePath = req.substr(httpMethodPosition + 2, tests - 5); // not sure why this is finding two more char after the space
    return filePath;
}

// std::string sendHandShake()
// {
//     std::string key = getWebsocketAcceptKey();
//     std::cout << "sending websocket respond" << std::endl;
//     // std::string response = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + key + "\r\nc\r\n";
//     std::string response =
//         "HTTP/1.1 101 Switching Protocols\r\n"
//         "Upgrade: websocket\r\n"
//         "Connection: Upgrade\r\n"
//         "Sec-WebSocket-Accept: " +
//         key + "\r\n"
//               "\r\n";

//     return response;
// }
// delete after websocket would be ready
// bool validWebSocketConnection()
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
//     return false;
// }

void HttpParser::splitToLines(std::string req)
{
    // read about stringstream more https://www.geeksforgeeks.org/stringstream-c-applications/
    auto ss = std::stringstream{req};
    string line;
    while (std::getline(ss, line, '\n'))
    {
        requestLines.push_back(line);
    }
}

string HttpParser::getLine(std::string keyWord)
{
    regex r("\s*" + keyWord + "\s*.*"); // entire match will be 2 numbers
    smatch m;
    regex_search(req, m, r);
    auto v = m[0];
    return v;
}

string HttpParser::getReqLineValue(std::string keyWord)
{
    std::string firstWordLine = getLine(keyWord);
    auto wordIndex = firstWordLine.find(" ");
    std::string requiredValue = firstWordLine.substr(wordIndex + 1);
    return requiredValue;
}
