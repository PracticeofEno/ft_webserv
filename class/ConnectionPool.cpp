#include "ConnectionPool.hpp"

ConnectionPool::ConnectionPool(int epfd) : epfd_(epfd)
{

}

ConnectionPool::~ConnectionPool()
{

}

void ConnectionPool::addConnection(int socket, int indicate)
{
    struct epoll_event userevent;      // 등록하기 위한 변수!
    Connection con(socket, indicate);

    this->cons_.push_back(con);
    
    int flags = fcntl(socket, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(socket, F_SETFL, flags) < 0)
    {
        std::cout << "server_fd fcntl() error" << std::endl;
        close(socket);
        _exit(1);
    }

    // 클라이언트의 fd를 등록해주는 과정
    userevent.events = EPOLLIN | EPOLLET;
    userevent.data.fd = socket;
    epoll_ctl(this->epfd_, EPOLL_CTL_ADD, socket, &userevent);
    std::cout << "connected client : " << socket << std::endl;
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