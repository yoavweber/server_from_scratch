#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
// #include <sstream>
// #include <stdlib.h>
// #include <iterator>
// #include <regex>
// #include <bits/stdc++.h>
// #include <openssl/sha.h>
// #include <bitset>
// #include "base64.cpp"

#ifndef HttpParser_H // include guard
#define HttpParser_H

// parse the HTTP request
namespace parser
{
    class HttpParser
    {
    public:
        explicit HttpParser(std::string req)
            : req(req){};

        // delete all comments once websocket is completed
        std::string getHeader();

        std::string getPath();

    protected:
        std::vector<std::string> requestLines;
        std::string req;

        void splitToLines(std::string req);

        std::string getLine(std::string keyWord);

        std::string getReqLineValue(std::string keyWord);
    };
} // namespace parser

#endif
