#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Total.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <sys/time.h>

class MainServer;

class Connection
{
public:
    int socket_;
    int kind_;
    long timeout_;
    int port_;
    int pipe_read_;
    Request reqeust_;
    Response response_;
    std::string client_ip_;
    std::string buf_cgi_;
    char buf_CGI_[BUF_SIZE];
    bool disconnect_;
    
    Connection();
    Connection(int socket, int kind, int port);
    ~Connection();
    Connection(const Connection& con);
    Connection &operator=(const Connection &tmp);

    bool makeRequest();
    std::string readLine();
    void resetData();
private:
};

#endif
