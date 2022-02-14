#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include <iostream>
#include <string>
#include <sstream>

class Server
{
    public :
        std::string             serverName;
        std::string             errorPagePath;
        std::string             root;
        int                     clientBodySize;
        std::vector<Location>   locations;
        int                     socket;
        int                     port;

        Server();
        ~Server();
        void dataSetting(std::string data);
        
    private :
};

#endif