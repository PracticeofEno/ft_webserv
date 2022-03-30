#include "Request.hpp"
#include "ExceptionCode.hpp"
#include "Util.hpp"
#include "Server.hpp"

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
    this->body_ = tmp.body_;
    this->query_ = tmp.query_;
    this->_buffer = tmp._buffer;
    this->state = tmp.state;
    this->location_ = tmp.location_;
    this->file_ = tmp.file_;
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
    int strlen;

    while (true)
    {
        strlen = read(socket, buf, BUF_SIZE);
        if (strlen == 0)
        {
            std::cout << "fd : " << socket << " all data receive" << std::endl;
            break;
        }
        else if (strlen > 0)
        {
            _buffer.append(buf, strlen);
            continue;
        }
        else
            break;
    }

    int tmp = 0;
    tmp = 5;
    (void)tmp;
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

void Request::parseStartline(std::string tmp)
{
    size_t endPos;

    if (tmp.compare("\r\n") == 0 && method_ == "GET")
    {
        state = DONE_REQUST;
        return ;
    }
    endPos = tmp.find(" ");
    if (endPos != std::string::npos)
    {
        method_ = tmp.substr(0, endPos);
        tmp.erase(0, endPos + 1);
        if (checkMethod(method_) == false)
        {
            ExceptionCode ex(405);
            ex.error_str = "Method Not Allowed";
            throw ex;
        }
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
        endPos = tmp.find("\r\n");
        if (endPos != std::string::npos)
        {
            method_ = tmp.substr(0, endPos);
            tmp.erase(0, endPos + 2);
            if (checkMethod(method_) == false)
            {
                ExceptionCode ex(405);
                ex.error_str = "Method Not Allowed";
                throw ex;
            }
            url_ = "/";
            version_ = "HTTP/1.1";
        }
    }
    state = HEADERS;
}

void Request::parseHeaders(std::string tmp)
{
    size_t endPos;
    std::string key;
    std::string value;

    if (tmp.compare("\r\n") == 0)
    {
        if (header_.find("Host") == header_.end())
        {
            ExceptionCode ex(400);
            ex.error_str = "Host header is not found";
            throw ex;
        }
        else
            state = BODY;
    }
    else
    {
        endPos = tmp.find(": ");
        if (endPos != std::string::npos)
        {
            key = tmp.substr(0, endPos);
            tmp.erase(0, endPos + 2);
            tmp = ft_rtrim(tmp, "\r\n");
            value = tmp;
            header_.insert(std::pair<std::string, std::string>(key, value));
        }
        else
        {
            ExceptionCode ex(404);
            throw ex;
        }
    }
}

void Request::parseChunked(std::string tmp)
{
    std::stringstream str;
    static int num;

    if (num == 0)
    {
        str.str(tmp);
        str >> num;
        if (num == 0 && tmp.compare("\r\n") == 0)
            state = DONE_REQUST;
    }
    else
    {
        body_.append(tmp.substr(0, num));
        num = 0;
    }
}

void Request::parseBody(std::string tmp)
{
    if (header_.find("Content-Length") == header_.end() && header_.find("Transfer-Encoding") == header_.end())
    {
        body_ = "";
        state = DONE_REQUST;
    }
    else if (header_.find("Transfer-Encoding") != header_.end())
    {
        state = CHUNKED;
    }
    else if (header_.find("Content-Length") != header_.end())
    {
        int length;
        std::istringstream convert(header_["Content-Length"]);
        convert >> length;
        if (_buffer != "")
            state = DONE_REQUST;
        body_ = _buffer.substr(0, length);
        //read content length
    }
}

bool Request::parseSocket()
{
    std::string tmp;

    while ((tmp = readLine()).compare("") != 0)
    {
        if (state == START_LINE)
        {
            parseStartline(tmp);
        }
        else if (state == HEADERS)
        {
            parseHeaders(tmp);
        }
        else if (state == BODY)
        {
            parseBody(tmp);
        }
        else if (state == CHUNKED)
        {
            parseChunked(tmp);
        }
        else if (state == BODY)
        {

        }
        else if (state == DONE_REQUST)
        {
            break;
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
    body_.clear();
    query_.clear();
    _buffer.clear();
    location_.clear();
    file_.clear();
}

void Request::printStartLine()
{
    std::cout << this->method_ << " | " << this->url_ << " | " << this->version_ << std::endl;
}

int Request::checkPostType()
{
    if (this->header_.find("Content-Type") == this->header_.end())
        return (NONE);
    std::string& value = this->header_["Content-Type"];
    if (value.find("multipart/form-data") == 0)
        return (UPLOAD_POST);
    else
        return (STATIC_POST);
}

void Request::setLocationFile()
{
    location_ = url_.substr(0, url_.find_last_of("/"));
    if (location_ == "")
        location_ = "/";
    file_ = url_.substr(url_.find_last_of("/") + 1, std::string::npos);
}
