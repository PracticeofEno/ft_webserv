#ifndef SERVER_HPP
#define SERVER_HPP

#include "Total.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "Request.hpp"

class Server
{
    public :
        std::string             server_name_;
        std::string             http_version_;
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
        Response handleRequest(Request& request);
        std::string getPayload(std::string path);
        
    private :
        int findLocation(std::string root);
        std::string generateTime();
        
};

#endif