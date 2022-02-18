#ifndef CONNECTIONPOOL_HPP
#define CONNECTIONPOOL_HPP

#include "Total.hpp"

class ConnectionPool
{
    public:
        int epfd_;
        std::vector<Connection> cons_;

        ConnectionPool();
        ConnectionPool(int epfd);
        ~ConnectionPool();

        void addConnection(int socket, int indicate);
        void deleteConnection(int socket);
        bool CheckServerSocket(int socket);

    private:
        
};

#endif