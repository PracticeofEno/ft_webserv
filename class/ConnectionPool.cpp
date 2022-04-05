#include "ConnectionPool.hpp"
#include "algorithm"

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
        userevent.events = EPOLLIN;
    else if (kind == CLIENT || kind == CGI)
        userevent.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLERR | EPOLLRDHUP;

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

void ConnectionPool::appConnection(int pipe, int kind)
{
    struct epoll_event userevent;      // 등록하기 위한 변수!

    int flags = fcntl(pipe, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(pipe, F_SETFL, flags) < 0)
    {
        std::cout << "server_fd fcntl() error" << std::endl;
        close(pipe);
        _exit(1);
    }

    if (kind == SERVER )
        userevent.events = EPOLLIN | EPOLLET | EPOLLERR | EPOLLRDHUP;
    else if (kind == CLIENT || kind == CGI)
        userevent.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLERR | EPOLLRDHUP;

    userevent.data.fd = pipe;
    epoll_ctl(this->epfd_, EPOLL_CTL_ADD, pipe, &userevent);
}

void ConnectionPool::deleteConnection(Connection& con)
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator its = cons_.begin();
    std::vector<Connection>::iterator ite = cons_.end();

    for (it = its; it != ite; it++)
    {
        if (it->socket_ == con.socket_)
            break;
    }
    if (it != ite)
    {
        std::cout << std::endl;
        this->printPool();
        std::cout << "delete connection : " << con.socket_ << std::endl;
        close(con.socket_);
        epoll_event ep_event;
        ep_event.data.fd = con.socket_;
        epoll_ctl(this->epfd_, EPOLL_CTL_DEL, con.socket_, &ep_event);
        this->cons_.erase(it);
        this->printPool();
        std::cout << std::endl;
    }
}

bool ConnectionPool::checkSocket(int socket, int kind)
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
        else if (kind == ALL)
            return true;
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
            break;
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
        if (it->socket_ == pipe)
            break;
    }
    if (it != ite)
    {
        std::cout << std::endl;
        this->printPool();
        std::cout << "delete pipe connection : " << pipe << std::endl;
        close(pipe);
        epoll_event ep_event;
        ep_event.data.fd = pipe;
        epoll_ctl(this->epfd_, EPOLL_CTL_DEL, pipe, &ep_event);
        this->cons_.erase(it);
        this->printPool();
        std::cout << std::endl;
    }
}

void ConnectionPool::printPool()
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator its = cons_.begin();
    std::vector<Connection>::iterator ite = cons_.end();
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

struct less_than_key
{
    inline bool operator() (const Connection& struct1, const Connection& struct2)
    {
        return (struct1.timeout_ < struct2.timeout_);
    }
};

void ConnectionPool::sort()
{
    std::sort(this->cons_.begin(), this->cons_.end(), less_than_key());
}

void ConnectionPool::eraseTimeOut(long decrease_time)
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator ite = cons_.end();

    it = cons_.begin();
    ite = cons_.end();
    while (true)
    {
        if (it == ite)
            break;
        if (it->kind_ == CLIENT)
        {
            it->timeout_ = it->timeout_ - decrease_time;
            if (it->timeout_ < 0)
            {
                std::cout << "time out delete fd : " << it->socket_ << std::endl;
                deleteConnection(*it);
            }
        }
        it++;
    }
}

long ConnectionPool::getMinTimeOut()
{
    std::vector<Connection>::iterator it;
    std::vector<Connection>::iterator its = cons_.begin();
    std::vector<Connection>::iterator ite = cons_.end();
    long min = 60000;

    for (it = its; it != ite; it++)
    {
        if (it->kind_ == CLIENT && it->timeout_ < min)
        {
            min = it->timeout_;
        }
    }
    return min;
}