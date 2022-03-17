#include "Request.hpp"
#include "ExceptionCode.hpp"

Request::Request(void) : state(0) {}

Request::~Request(void) {}

Request::Request(const Request& tmp)
{
    *this = tmp;
}

Request& Request::operator= (const Request& tmp)
{
    this->url_ = tmp.url_;
    this->method_ = tmp.method_;
    this->header_ = tmp.header_;
    this->version_ = tmp.version_;
    this->query_ = tmp.query_;
    this->_buffer = tmp._buffer;
    this->state = tmp.state;
    return *this;
}

int Request::getState()
{
    return this->state;
}

std::string Request::readLine()
{
    std::string ret("");
    size_t      index = _buffer.find("\r\n");
    if (index != std::string::npos)
    {
        ret = _buffer.substr(0, index + 2);
        _buffer.erase(0, index + 2);
    }
    return (ret);
}

void Request::readSocket(int socket)
{
    char buf[BUF_SIZE];
    int strLen;

    while (true)
    {
        strLen = read(socket, buf, BUF_SIZE);
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

bool Request::checkMethod(std::string method)
{
    if (method == "GET" || method == "POST" || method == "DELETE")
        return true;
    return false;
}

bool Request::parseSocket()
{
    size_t endPos;

    if (method_ == "")
    {
        endPos = _buffer.find(" ");
        method_ = _buffer.substr(0, endPos);
        _buffer.erase(0, endPos + 1);
        if (checkMethod(method_) == false)
        {
            throw ExceptionCode(405);
        }
        endPos = _buffer.find(" ");
        url_ = _buffer.substr(0, endPos);
        _buffer.erase(0, endPos + 1);
        if (url_ == "")
        {
            url_ = "/index.html";
            version_ = "HTTP/1.1";
            return true;
        }
        endPos = _buffer.find("\r\n");
        version_ = _buffer.substr(0, endPos);
        _buffer.erase(0, endPos + 2);
        if (version_ == "")
        {
            version_ = "HTTP/1.1";
            return true;
        }
    }
    else
    {
        std::string host;
        std::string server;

        endPos = _buffer.find(": ");
        host = _buffer.substr(0, endPos);
        _buffer.erase(0, endPos + 2);
        endPos = _buffer.find("\r\n");
        server = _buffer.substr(0, endPos);
        _buffer.erase(0, endPos + 2);
        header_.insert(std::pair<std::string, std::string>(host, server));
    }
    return true;
}

void Request::resetData()
{

}
