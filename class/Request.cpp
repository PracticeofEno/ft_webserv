#include "Request.hpp"
#include "ExceptionCode.hpp"

Request::Request(void) : state(START_LINE) {}

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
        ret = _buffer.substr(0, index);
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

bool Request::checkUrl(std::string url)
{
    if (url == "" || url == "/" || url == "/index.html")
        return true;
    return false;
}

bool Request::checkVersion(std::string version)
{
    if (version == "" || version == "HTTP/1.1")
        return true;
    return false;
}

bool Request::parseSocket()
{
    return true;
}

void Request::resetData()
{
    state = START_LINE;
    method_.clear();
    url_.clear();
    version_.clear();
    header_.clear();
}
