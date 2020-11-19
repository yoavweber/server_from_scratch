#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <map>
#include "../net/socket.h"

#include "responses.h"

using namespace http;
using namespace std;

HttpResponses::HttpResponses()
{
}

string HttpResponses::successResponse(string route)
{
    string successResponse = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
    return httpRes(successResponse, route);
}

string HttpResponses::response404()
{
    string Response404 = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";
    string filePath = "static/404.html";
    return httpRes(Response404, filePath);
}

string HttpResponses::httpRes(string res, string filePath)
{
    ifstream file;
    file.open(filePath);
    int size = file.tellg();
    cout << filePath << endl;
    string content((istreambuf_iterator<char>(file)),
                   (istreambuf_iterator<char>()));
    int contentLengthInt = content.size();
    string contentLength = to_string(contentLengthInt);
    string response = res + contentLength + "\n\n";
    response.append(content);

    file.close();
    return response;
}
