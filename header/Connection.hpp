#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Total.hpp"
#include "Request.hpp"

class Connection
{
public:
    int socket_;
    int kind_;
    int timeout_;
    Request reqeust_;

    Connection(int socket, int kind);
    ~Connection();
    Connection(const Connection& con);
    Connection &operator=(const Connection &tmp);

   
    void read();

private:
};

#endif