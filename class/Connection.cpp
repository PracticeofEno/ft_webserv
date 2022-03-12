#include "Connection.hpp"
#include "MainServer.hpp"

Connection::Connection() {}
Connection::Connection(int socket, int kind) : socket_(socket), kind_(kind), timeout_(0) {}

Connection::~Connection() {}

Connection::Connection(const Connection &tmp)
{
    *this = tmp;
}

Connection &Connection::operator=(const Connection &tmp)
{
    this->socket_ = tmp.socket_;
    this->kind_ = tmp.kind_;
    this->timeout_ = tmp.timeout_;
    this->reqeust_ = tmp.reqeust_;
    this->response_ = tmp.response_;
    this->client_ip_ = tmp.client_ip_;
    this->file_fd_ = tmp.file_fd_;
    this->pipe_fd[0] = tmp.pipe_fd[0];
    this->pipe_fd[1] = tmp.pipe_fd[1];
    return *this;
}

bool Connection::makeRequest()
{
    this->reqeust_.readSocket(this->socket_);
    
    std::cout << this->reqeust_._buffer << std::endl;
    return true;
}

void Connection::addFile(MainServer& tmp)
{
    pipe(pipe_fd);
    this->file_fd_ = open(this->response_.file_path_.c_str(), O_RDONLY);
    tmp.pipe_to_fd_[this->pipe_fd[0]] = this->file_fd_;
    tmp.cons_.addConnection(pipe_fd[0], FILE_READ, "FILE");
    write(pipe_fd[1], "a", 1);
}