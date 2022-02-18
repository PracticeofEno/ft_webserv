#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Total.hpp"

class Connection 
{
    public:
        int socket_;
        int kind_;
        int timeout_;
        
        Connection(int socket, int kind);
        ~Connection();
        Connection(const Connection& con);
        Connection& operator=(const Connection& con);

    private:
        
};

#endif