#ifndef SERVERPOOL_HPP
#define SERVERPOOL_HPP

#include "Total.hpp"
#include "Server.hpp"

class ServerPool
{
    public :
        std::vector<Server> serverPool_;

        ServerPool();
        ~ServerPool();
        ServerPool(const ServerPool& tmp);
        ServerPool& operator=(const ServerPool& tmp);
    private :

};
#endif