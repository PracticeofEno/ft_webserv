#include "ConnectionPool.hpp"

ConnectionPool::ConnectionPool()
{

}

ConnectionPool::~ConnectionPool()
{

}

ConnectionPool::ConnectionPool(const ConnectionPool& tmp)
{
    *this = tmp;
}

ConnectionPool& ConnectionPool::operator=(const ConnectionPool& tmp)
{
    this->epfd_ = tmp.epfd_;
    this->cons_ = tmp.cons_;
    return *this;
}

void ConnectionPool::addConnection(int socket, int kind, std::string client_ip, int port)
{
    struct epoll_event userevent;      // 등록하기 위한 변수!

    Connection con(socket, kind, port);
    con.client_ip_ = client_ip;

    this->cons_.push_back(con);
    
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(socket, F_SETFL, flags) < 0)
    {
        std::cout << "add connection fcntl() error" << std::endl;
        close(socket);
        _exit(1);
    }

    if (kind == SERVER )
        userevent.events = EPOLLIN | EPOLLET | EPOLLHUP | EPOLLERR;
    else if (kind == CLIENT || kind == CGI)
        userevent.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLHUP | EPOLLERR;

    userevent.data.fd = socket;
    epoll_ctl(this->epfd_, EPOLL_CTL_ADD, socket, &userevent);
    if (kind == SERVER)
        std::cout << "make server : " << socket << std::endl;
    else if (kind == CLIENT)
        std::cout << "connected client : " << socket << std::endl;
    else if (kind == CGI)
    {
        std::cout << "cons_ add pipe_fd : " << socket << std::endl;
    }
}

void ConnectionPool::appConnection(Connection &con, int socket, int kind, std::string client_ip)
{
    struct epoll_event userevent;      // 등록하기 위한 변수!

    con.client_ip_ = client_ip;
    
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(socket, F_SETFL, flags) < 0)
    {
        std::cout << "server_fd fcntl() error" << std::endl;
        close(socket);
        _exit(1);
    }

    if (kind == SERVER )
        userevent.events = EPOLLIN | EPOLLET | EPOLLHUP | EPOLLERR;
    else if (kind == CLIENT || kind == CGI)
        userevent.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLHUP | EPOLLERR;

    userevent.data.fd = socket;
    epoll_ctl(this->epfd_, EPOLL_CTL_ADD, socket, &userevent);
}

void ConnectionPool::deleteConnection(int socket)
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator its = cons_.begin();
    std::vector<Connection>::iterator ite = cons_.end();

    for (it = its; it != ite; it++)
    {
        if (it->socket_ == socket)
            break;
    }
    if (it != ite)
    {
        std::cout << "delete connection : " << it->socket_ << std::endl;
        close(it->socket_);
        cons_.erase(it);
    }
    epoll_event ep_event;
    ep_event.data.fd = socket;
    epoll_ctl(this->epfd_, EPOLL_CTL_DEL, socket, &ep_event);
}

bool ConnectionPool::CheckSocket(int socket, int kind)
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator its = cons_.begin();
    std::vector<Connection>::iterator ite = cons_.end();

    for (it = its; it != ite; it++)
    {
        if (it->socket_ == socket)
            break;
    }
    if (it != ite)
    {
        if (it->kind_ == kind)
            return (true);
        else
            return (false);
    }
    return (false);
}

Connection& ConnectionPool::getConnection(int socket)
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator its = cons_.begin();
    std::vector<Connection>::iterator ite = cons_.end();

    for (it = its; it != ite; it++)
    {
        if (it->socket_ == socket || it->pipe_read_ == socket)
        {
            return *it;
        }
    }
    return *it;
}

void ConnectionPool::setEpfd(int epfd)
{
    this->epfd_ = epfd;
}


void ConnectionPool::deletePipeConnection(int pipe)
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator its = cons_.begin();
    std::vector<Connection>::iterator ite = cons_.end();

    for (it = its; it != ite; it++)
    {
        if (it->pipe_read_ == pipe)
            break;
    }
    if (it != ite)
    {
        std::cout << "delete connection : " << it->socket_ << std::endl;
        cons_.erase(it);
    }
    epoll_event ep_event;
    ep_event.data.fd = pipe;
    epoll_ctl(this->epfd_, EPOLL_CTL_DEL, pipe, &ep_event);
    close(pipe);
}

void ConnectionPool::printPool()
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator its = cons_.begin();
    std::vector<Connection>::iterator ite = cons_.end();

    std::cout << std::endl << std::endl;
    for (it = its; it != ite; it++)
    {
        std::cout << "fd : " << it->socket_ << " | kinds = ";
        if (it->kind_  == SERVER) 
            std::cout << "SERVER" << std::endl;
        else if (it->kind_  == CLIENT) 
            std::cout << "CLIENT" << std::endl;
        else if (it->kind_  == CGI) 
            std::cout << "CGI" << std::endl;
    }
}