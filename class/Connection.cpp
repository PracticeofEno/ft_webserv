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

void Connection::read(int socket)
{
    std::string tmp;
    char buf[bufSize];
    int strLen;

    while (true)
    {
        strLen = read(socket, buf, bufSize);
        if (strLen < 0)    
        {
            if (errno == EAGAIN)
                break;
        }
        else if (strLen == 0)
        {
            std::cout << "all data receive" << std::endl;
            break;
        }
        else
        {
            tmp.append(buf, strLen);
        }
    }
    return tmp;
}

std::string Connection::readLine(int socket)
{
    
}