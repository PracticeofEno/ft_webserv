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
    std::vector<Server>::iterator its = serverPool_.begin();
    std::vector<Server>::iterator ite = serverPool_.end();
    std::vector<int>::iterator p;
    std::vector<int>::iterator ps;
    std::vector<int>::iterator pe;

    bool first = true;

    for (it = its; it != ite; it++)
    {
        ps = (it->port_).begin();
        pe = (it->port_).end();

        for (p = ps; p != pe; p++)
        {
            if (*p == port)
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