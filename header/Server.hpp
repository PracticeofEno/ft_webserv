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
#define NONE 0

class Server
{
    public :
        std::string             server_name_;
        std::string             http_version_;
        int                     client_body_size_;
        std::vector<Location>   locations_;
        int                     socket_;
        std::vector<int>        port_;
        std::string             error_page_;

        /*                      OCCF                             */
        Server();
        ~Server();
        Server(const Server& server);
        Server& operator=(const Server& server);
        //////////////////////////////////////////////////////////

        void dataSetting(std::string data);
        Response handleRequest(Request& request, Connection& tmp);
        Response handleRequestCGI(Connection& tmp);
        Location& findLocation(Request& reqeust);
        bool CheckCGI(std::string url, Location& location);
        void CGIHandler(Request& request, Connection& tmp, Location& location);
        
    private :
        Response GETHandler(Request& request, Location& location);
        Response POSTHandler(Request& request, Location& location);
        Response DELETEHandler(Request& request, Location& location);
        Response GETHandlerCGI(Response& res);
        Response POSTHandlerCGI(Request& request, Location& location);
        Response DELETEHandlerCGI(Request& request, Location& location);
        char** getCgiVariable(Request& request, Connection& tmp, Location& location);
        std::string getCgiUri(Request& req, int port);

};

#endif