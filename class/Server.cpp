#include "Server.hpp"

std::string &ltrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

std::string &rtrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

std::string &trim(std::string &s, const char *t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}

Server::Server()
{

}

Server::~Server()
{
    
}

void Server::dataSetting(std::string data)
{
    std::string key, value;
    data = trim(data);
    key = data.substr(0, data.find_first_of(' '));
    data = data.erase(0, data.find_first_of(' '));
    value = trim(data);
    if (key.compare("serverName") == 0)
        serverName = value;
    else if (key.compare("errorPagePath") == 0)
        errorPagePath = value;
    else if (key.compare("root") == 0)
        root = value;
    else if (key.compare("port") == 0)
        std::stringstream(value) >> port;
    else if (key.compare("clientBodySize") == 0)
        std::stringstream(value) >> clientBodySize;
}