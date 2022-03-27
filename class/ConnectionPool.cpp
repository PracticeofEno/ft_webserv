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
    if (kind == SERVER)
        std::cout << "make server : " << socket << std::endl;
    else if (kind == CLIENT)
        std::cout << "connected client : " << socket << std::endl;
    else if (kind == CGI)
    {
        std::cout << "cons_ add pipe_fd : " << socket << std::endl;
    }
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
        cons_.erase(it);
    epoll_ctl(this->epfd_, EPOLL_CTL_DEL, socket, NULL);
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
            return *it;
    }
    return *it;
}

void ConnectionPool::setEpfd(int epfd)
{
    this->epfd_ = epfd;
}
