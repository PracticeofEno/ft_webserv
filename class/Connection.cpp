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

bool Connection::makeRequest()
{
    readSocket();
    
    std::cout << this->_buffer << std::endl;
    return true;
}

void Connection::readSocket()
{
    char buf[BUF_SIZE];
    int strLen;

    while (true)
    {
        strLen = read(socket_, buf, BUF_SIZE);
        if (strLen == 0)
        {
            std::cout << "all data receive" << std::endl;
            break;
        }
        else if (strLen > 0)
        {
            _buffer.append(buf, strLen);
            continue;
        }
        else
            break;
    }
}

std::string Connection::readLine()
{
    /*
    std::string str = "I want to convert string to char*";
    std::vector<char> writable(_buffer.begin(), _buffer.end());
    writable.push_back('\0');
    char *ptr = &writable[0];
    std::cout << ptr;
    */
    std::string ret("");
    size_t      index = _buffer.find("\r\n");
    if (index != std::string::npos)
    {
        ret = _buffer.substr(0, index + 2);
        _buffer.erase(0, index + 2);
    }
    return (ret);
}