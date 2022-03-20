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

    while (_buffer != "")
    {
        if (state == FILL_START_LINE)
        {
            if (_buffer.compare("\r\n") == 0)
                state = FILL_HEADERS;
            else
            {
                std::string host;
                std::string server;

                endPos = _buffer.find(": ");
                if (endPos != std::string::npos)
                {
                    host = _buffer.substr(0, endPos);
                    _buffer.erase(0, endPos + 2);
                    server = _buffer;
                    _buffer.clear();
                    header_.insert(std::pair<std::string, std::string>(host, server));
                }
<<<<<<< Updated upstream
=======
                //std::cout << _buffer << std::endl;
>>>>>>> Stashed changes
            }
        }
        else if (state == FILL_HEADERS)
        {
            _buffer.clear();
            state = FILL_REQUEST;
        }
<<<<<<< Updated upstream
        else
=======
    }
    else // 스타트 라인이 정상적으로 들어온 이후
    {
        if (_buffer.compare("\r\n") == 0)
        {
            state = DONE_REQUEST;
            return true;
        }

        std::string host;
        std::string server;

        endPos = _buffer.find(": ");
        //std::cout << "buffer : " << _buffer << std::endl;
        //std::cout << host << " : " << server << std::endl;
        if (endPos != std::string::npos)
>>>>>>> Stashed changes
        {
            endPos = _buffer.find(" ");
            if (endPos != std::string::npos)
            {
                method_ = _buffer.substr(0, endPos);
                _buffer.erase(0, endPos + 1);
                if (checkMethod(method_) == false)
                    throw ExceptionCode(405);
                url_ = "/";
                version_ = "HTTP/1.1";
                endPos = _buffer.find(" ");
                if (endPos != std::string::npos)
                {
                    url_ = _buffer.substr(0, endPos);
                    _buffer.erase(0, endPos + 1);
                    if (checkUrl(url_) == false)
                        throw ExceptionCode(404);
                    version_ = _buffer;
                    _buffer.clear();
                }
            }
            else
            {
                method_ = _buffer;
                _buffer.clear();
                if (checkMethod(method_) == false)
                    throw ExceptionCode(405);
                url_ = "/";
                version_ = "HTTP/1.1";
            }
            state = FILL_START_LINE;
        }
    }
    return true;
}

// bool Request::parseSocket()
// {
//     size_t endPos;

//     if (method_ == "")
//     {
//         endPos = _buffer.find(" ");
//         if (endPos != std::string::npos) // 공백이 있을 경우
//         {
//             method_ = _buffer.substr(0, endPos);
//             _buffer.erase(0, endPos + 1);
//             if (checkMethod(method_) == false)
//                 throw ExceptionCode(405);
//             url_ = "/index.html";
//             version_ = "HTTP/1.1";
//             endPos = _buffer.find(" ");
//             if (endPos != std::string::npos)
//             {
//                 url_ = _buffer.substr(0, endPos);
//                 _buffer.erase(0, endPos + 1);
//                 if (url_ == "")
//                 {
//                     url_ = "/";
//                     version_ = "HTTP/1.1";
//                 }
//                 endPos = _buffer.find("\r\n");
//                 if (endPos != std::string::npos)
//                 {
//                     version_ = _buffer.substr(0, endPos);
//                     _buffer.erase(0, endPos + 2);
//                     if (version_ == "")
//                         version_ = "HTTP/1.1";
//                 }
//                 std::cout << _buffer << std::endl;
//             }
//         }
//         else // 스타트 라인이 비정상적으로 들어왔거나 method만 들어온 경우
//         {
//             endPos = _buffer.find("\r\n");
//             if (endPos == std::string::npos)
//             {
//                 throw ExceptionCode(405);
//             }
//             method_ = _buffer.substr(0, endPos);
//             _buffer.erase(0, endPos + 2);
//             if (checkMethod(method_) == false)
//                 throw ExceptionCode(405);
//             url_ = "/";
//             version_ = "HTTP/1.1";
//         }
//     }
//     else // 스타트 라인이 정상적으로 들어온 이후
//     {
//         if (_buffer.compare("\r\n") == 0)
//         {
//             state = DONE_REQUEST;
//             return true;
//         }

//         std::string host;
//         std::string server;

//         endPos = _buffer.find(": ");
//         if (endPos != std::string::npos)
//         {
//             host = _buffer.substr(0, endPos);
//             _buffer.erase(0, endPos + 2);
//             endPos = _buffer.find("\r\n");
//             if (endPos != std::string::npos)
//             {
//                 server = _buffer.substr(0, endPos);
//                 _buffer.erase(0, endPos + 2);
//                 header_.insert(std::pair<std::string, std::string>(host, server));
//             }
//         }
//         else
//             state = DONE_REQUEST;
//     }
//     return true;
// }

void Request::resetData()
{
    state = 0;
    method_.clear();
    url_.clear();
    version_.clear();
    header_.clear();
}
