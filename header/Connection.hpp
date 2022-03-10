#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#define START_LINE 11
#define HEADER 22
#define BODY 33

#include "Total.hpp"
#include "Request.hpp"

//epoll에 등록한 fd

class Connection
{
public:
    int socket_; // Clinet socket
    int kind_;
    int timeout_;
    Request reqeust_;
    int epollfd;
    std::string client_ip_;

    Connection(int socket, int kind);
    ~Connection();
    Connection(const Connection& con);
    Connection &operator=(const Connection &tmp);

    bool makeRequest();
    void readSocket();
    std::string readLine();

private:
    std::string _buffer;

};

#endif