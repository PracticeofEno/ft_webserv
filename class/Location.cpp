#include "Location.hpp"

std::string &ltrim(std::string &s)
{
    s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
    return s;
}

std::string &rtrim(std::string &s)
{
    s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
    return s;
}

std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

Location::Location()
{
}

Location::~Location()
{
}

void Location::dataSetting(std::string data)
{
    std::string key, value;
    data = trim(data);
    key = data.substr(0, data.find_first_of(' '));
    data = data.erase(0, data.find_first_of(' '));
    value = trim(data);
    if (key.compare("method") == 0)
        method = value;
    else if (key.compare("redirection") == 0)
        redirection = value;
    else if (key.compare("root") == 0)
        root = value;
    else if (key.compare("cgiExtension") == 0)
        cgiExtension = value;
    else if (key.compare("uploadPath") == 0)
        uploadPath = value;
    else if (key.compare("dlDefault") == 0)
        dlDefault = value;
    else if (key.compare("directoryListing") == 0)
    {
        directoryListing = false;
        if (value.compare("on") == 0)
            directoryListing = true;
        else if (value.compare("off") == 0)
            directoryListing = false;
    }
}