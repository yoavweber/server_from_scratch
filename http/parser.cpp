#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <regex>
#include <bitset>
#include "parser.h"

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
