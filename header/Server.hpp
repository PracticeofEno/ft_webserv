#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"

class Server
{
    public :
        std::string             serverName;
        std::string             errorPagePath;
        int                     clientBodySize;
        std::vector<Location>   locations;
        int                     socket;
        int                     port;

        Server();
        ~Server();
        
    private :
};

#endif