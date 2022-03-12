#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#define START_LINE 11
#define HEADER 22
#define BODY 33

#include "Total.hpp"
#include "Request.hpp"
#include "Response.hpp"
class MainServer;

class Connection
{
public:
    int socket_; // Clinet socket
    int kind_;
    int timeout_;
    Request reqeust_;
    Response response_;
    std::string client_ip_;
    int pipe_fd[2];
    int file_fd_;

    Connection();
    Connection(int socket, int kind);
    ~Connection();
    Connection(const Connection& con);
    Connection &operator=(const Connection &tmp);

    bool makeRequest();
    void readSocket();
    std::string readLine();
    void addFile(MainServer& tmp);
private:
};

#endif