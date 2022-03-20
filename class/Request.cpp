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
    size_t endPos;
    std::string tmp;

    if (_buffer.compare("\r\n") == 0)
        state = DONE_REQUST;
    while ((tmp = readLine()).compare("") != 0)
    {
        if (state == START_LINE)
        {
            endPos = tmp.find(" ");
            if (endPos != std::string::npos)
            {
                method_ = tmp.substr(0, endPos);
                tmp.erase(0, endPos + 1);
                if (checkMethod(method_) == false)
                    throw ExceptionCode(405);
                url_ = "/";
                version_ = "HTTP/1.1";
                endPos = tmp.find(" ");
                if (endPos != std::string::npos)
                {
                    url_ = tmp.substr(0, endPos);
                    tmp.erase(0, endPos + 1);
                    version_ = tmp;
                }
            }
            else
            {
                method_ = tmp;
                if (checkMethod(method_) == false)
                    throw ExceptionCode(405);
                url_ = "/";
                version_ = "HTTP/1.1";
            }
            state = HEADERS;
        }
        else if (state == HEADERS)
        {
            if (tmp.compare("\r\n") == 0)
            {
                if (header_.size() == 0)  //헤더가 없고
                    throw ExceptionCode(400);
                else  //헤더가 있고
                {
                    if (header_.find("Content-Length") == header_.end() && header_.find("Transfer-Encoding") == header_.end())
                    {
                        state = DONE_REQUST;
                    }
                    else
                    {
                        state = BODY;
                    }
                }
            }
            else
            {
                std::string header_key;
                std::string header_value;

                endPos = tmp.find(": ");
                if (endPos != std::string::npos)
                {
                    header_key = tmp.substr(0, endPos);
                    tmp.erase(0, endPos + 2);
                    header_value = tmp;
                    header_.insert(std::pair<std::string, std::string>(header_key, header_value));
                }
                else
                    throw ExceptionCode(404);
            }
        }
        else if (state == BODY)
        {
            if (tmp.compare("\r\n") == 0)
                state = DONE_REQUST;
        }
    }
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
