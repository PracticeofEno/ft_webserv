#ifndef SERVER_HPP
#define SERVER_HPP

#include "Total.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "ExceptionCode.hpp"
#include "Util.hpp"
#include "Connection.hpp"
#include <limits.h>
#include <stdlib.h>

#define NOT_ALLOW_METHOD 0
#define ON 1
#define NO_EXIST_FILE 0
#define DIRECTORY 1

class Server
{
    public :
        std::string             server_name_;
        std::string             http_version_;
        std::string             error_page_path_;
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
        Response handleRequest(Request& request, Connection& tmp);
        
    private :
        Location& findLocation(std::string& root);
        Response GETHandler(Request& request, Location& location);
        Response POSTHandler(Request& request, Location& location);
        Response DELETEHandler(Request& request, Location& location);
        void CGIHandler(Request& request, Connection& tmp, Location& location);
        bool CheckCGI(std::string url, Location& location);
        char** getCgiVariable(Request& request, Connection& tmp, Location& location);
        std::string getCgiUri(Request& req);

};

#endif