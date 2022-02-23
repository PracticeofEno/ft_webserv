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

Server::Server(const Server& server)
{
    *this = server;
}

Server& Server::operator=(const Server& server)
{
    this->port_ = server.port_;
    this->root_ = server.root_;
    this->error_page_path_ = server.error_page_path_;
    this->client_body_size_ = server.client_body_size_;
    this->server_name_ = server.server_name_;
    this->socket_ = server.socket_;
    this->locations_ = server.locations_;
    return *this;
}

void Server::dataSetting(std::string data)
{
    std::string key, value;
    data = trim(data);
    key = data.substr(0, data.find_first_of(' '));
    data = data.erase(0, data.find_first_of(' '));
    value = trim(data);
    if (key.compare("serverName") == 0)
        server_name_ = value;
    else if (key.compare("errorPagePath") == 0)
        error_page_path_ = value;
    else if (key.compare("root") == 0)
        root_ = value;
    else if (key.compare("port") == 0)
        std::stringstream(value) >> port_;
    else if (key.compare("clientBodySize") == 0)
        std::stringstream(value) >> client_body_size_;
}

Response Server::handleRequest(Request& request)
{
   
}