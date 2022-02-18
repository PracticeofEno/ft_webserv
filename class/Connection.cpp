#include "Connection.hpp"

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
    return *this;
}