
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>
#include <string>

#ifndef RESPONSES_H // include guard
#define RESPONSES_H

namespace http
{
    class HttpResponses
    {

    public:
        // HttpResponses(std::ifstream);
        HttpResponses();
        std::string response404();
        std::string successResponse(std::string route);
        // send a response for files
    private:
        // std::ifstream &file;
        std::string httpRes(std::string res, std::string filePath);
    };
} // namespace http
#endif
