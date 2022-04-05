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
        void addConnection(int socket, int indicate, std::string client_ip, int port);
        void appConnection(int pipe, int kind);
        void deleteConnection(Connection& con);
        void deletePipeConnection(int pipe);
        bool checkSocket(int socket, int kind);
        void eraseTimeOut(long decrease_time);
        void sort();
        long getMinTimeOut();
        Connection& getConnection(int socket);

        void printPool();

    private:
        
};

#endif