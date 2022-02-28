#ifndef CONNECTIONPOOL_HPP
#define CONNECTIONPOOL_HPP

#include "Total.hpp"
#include "Connection.hpp"

class ConnectionPool
{
    public:
        int epfd_;
        std::vector<Connection> cons_;

        ConnectionPool();
        ConnectionPool(int epfd);
        ~ConnectionPool();

        void setEpfd(int epfd);
        void addConnection(int socket, int indicate);
        void deleteConnection(int socket);
        bool CheckSocket(int socket, int kind);
        Connection& getConnection(int socket);

    private:
        
};

#endif