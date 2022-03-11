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

#define MIMETYPE std::map<std::string, std::string>

class Server
{
    public :
        std::string             server_name_;
        std::string             http_version_;
        std::string             error_page_path_;
        std::string             root_;
        std::string             cgi_extension_;
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
        int findLocation(std::string root);
        std::string generateTime();
        Response GETHandler(Request& request);
        Response POSTHandler(Request& request);
        Response DELETEHandler(Request& request);
        void CGIHandler(Request& request, Connection& tmp);
        std::string searchMimeType(std::string uri);
        std::string getRecentTime(std::string url);
        std::string getFilePath(std::string url);
        std::string getFileSize(std::string url);
        Response tryHandle(Request& req, Connection& tmp);
        Response tryHandle(Request& req, int index, Connection& tmp);
        bool CheckCGI(std::string url);
        bool CheckCGI(std::string url, int index);
        char** getCgiVariable(Request& request, Connection& tmp);
        std::string getCgiUri(Request& req);

};

#endif