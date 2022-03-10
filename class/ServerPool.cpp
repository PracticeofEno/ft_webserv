#include "ServerPool.hpp"

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