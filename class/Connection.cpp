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
    return *this;
}

bool Connection::makeRequest()
{
    this->reqeust_.readSocket(this->socket_);
    std::cout << this->reqeust_._buffer << std::endl;
    this->reqeust_.parseSocket();

    //std::cout << this->reqeust_._buffer << std::endl;
    return true;
}

void Connection::resetData()
{
    response_.resetData();
    reqeust_.resetData();
}
