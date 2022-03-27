#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Total.hpp"
#include "Request.hpp"
#include "Response.hpp"

class MainServer;

class Connection
{
public:
    int socket_; // Clinet socket
    int kind_;
    int timeout_;
    int port_;
    int pipe_read_;
    int pipe_event_;
    Request reqeust_;
    Response response_;
    std::string client_ip_;
    char buf_CGI_[BUF_SIZE];
    
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
