#ifndef CONNECTIONPOOL_HPP
#define CONNECTIONPOOL_HPP

#include "Total.hpp"
#include "Connection.hpp"
#include <arpa/inet.h>

class ConnectionPool
{
    public:
        int epfd_;
        std::vector<Connection> cons_;

        ConnectionPool();
        ~ConnectionPool();
        ConnectionPool(const ConnectionPool& tmp);
        ConnectionPool& operator=(const ConnectionPool& tmp);


        void setEpfd(int epfd);
        void addConnection(int socket, int indicate, std::string client_ip);
        void deleteConnection(int socket);
        bool CheckSocket(int socket, int kind);
        Connection& getConnection(int socket);
        Connection& getPipeConnection(int pipe_fd);

    private:
        
};

#endif