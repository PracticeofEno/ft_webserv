#include "ServerPool.hpp"
#include <algorithm>

ServerPool::ServerPool()
{
}

ServerPool::~ServerPool()
{
}

ServerPool::ServerPool(const ServerPool& tmp)
{
    *this = tmp;
}

ServerPool& ServerPool::operator=(const ServerPool& tmp)
{
    this->serverPool_ = tmp.serverPool_;
    return *this;
}

Server& ServerPool::getServer(std::string host, int port)
{
    std::vector<Server>::iterator it;
    std::vector<Server>::iterator tmp;
    std::vector<int>::iterator port_iterator;

    bool first = true;
    for (it = serverPool_.begin(); it != serverPool_.end(); it++)
    {
        for (port_iterator = it->port_.begin(); port_iterator != it->port_.end(); port_iterator++)
        {
            if (*port_iterator == port)
            {
                if (first)
                {
                    tmp = it;
                    first = false;
                }
                if (it->server_name_ == host)
                    return (*it);
            }
        }
    }
    return (*tmp);
}