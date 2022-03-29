#include "ExceptionCode.hpp"
#include "MainServer.hpp"

ExceptionCode::ExceptionCode() {}
ExceptionCode::ExceptionCode(int code)
{
    status_ = ResponseStatus(code);
    _code = code;
}
ExceptionCode::ExceptionCode(int code, Connection &con) : _code(code)
{
    status_ = ResponseStatus(code);
    con_ = con;
}
ExceptionCode::ExceptionCode(const ExceptionCode &tmp)
{
    *this = tmp;
}

ExceptionCode &ExceptionCode::operator=(const ExceptionCode &tmp)
{
    this->con_ = tmp.con_;
    this->status_ = tmp.status_;
    this->_code = tmp._code;
    return *this;
}

ExceptionCode::~ExceptionCode() throw() {}
const char *ExceptionCode::what() const throw()
{
    return "Message";
}

int ExceptionCode::getCode() const
{
    return _code;
}

void ExceptionCode::handleException()
{
    Server &server = main_server.sp_.getServer(this->con_.reqeust_.header_["Host"], this->con_.port_);
    Response &res = this->con_.response_;
    Location &location = server.findLocation(con_.reqeust_);

    res.header_["Server"] = server.server_name_;
    res.header_["Date"] = generateTime();
    res.header_["Connection"] = "Keep-Alive";
    res.status_ = ResponseStatus(_code);
    res.http_version_ = "HTTP/1.1";

    if (_code == 404)
    {
        res.header_["Content-Type"] = "text/html";
        res.header_["Content-Length"] = location.getFileSize("/" + server.error_page_);
        res.file_path_ = location.getFilePath("/" + server.error_page_);
        res.send(con_.socket_);
    }
    else if (_code == 
}
