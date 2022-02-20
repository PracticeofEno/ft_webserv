#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include <iostream>
#include <string>
#include <sstream>

class Server
{
    public :
        std::string             server_name_;
        std::string             error_page_path_;
        std::string             root_;
        int                     client_body_size_;
        std::vector<Location>   locations_;
        int                     socket_;
        int                     port_;

        /*                      OCCF                             */
        Server();
        ~Server();
        Server(const Server& server);
        Server& operator=(const Server& server);
        //////////////////////////////////////////////////////////

        void dataSetting(std::string data);
        Response Server::handleRequest(Reqeust& request)
        
    private :
};

#endif