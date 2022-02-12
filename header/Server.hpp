#ifndef SERVER_HPP
#define SERVER_HPP

#include "total.hpp"

class Server
{
    public :
        std::string             serverName;
        std::string             errorPagePath;
        int                     clientBodySize;
        std::vector<Location>   locations;
        int                     socket;
        int                     port;
        
    private :
};

#endif