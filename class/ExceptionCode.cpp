#include "ExceptionCode.hpp"
#include "MainServer.hpp"

ExceptionCode::ExceptionCode() : con_(main_server.cons_.cons_[0]){}

ExceptionCode::ExceptionCode(int code) : con_(main_server.cons_.cons_[0])
{
    status_ = ResponseStatus(code);
    code_ = code;
}

ExceptionCode::ExceptionCode(int code, Connection &con) : con_(con), code_(code)
{
    status_ = ResponseStatus(code);
}

ExceptionCode::ExceptionCode(const ExceptionCode &tmp) : con_(tmp.con_)
{
    *this = tmp;
}

ExceptionCode &ExceptionCode::operator=(const ExceptionCode &tmp)
{
    this->con_ = tmp.con_;
    this->status_ = tmp.status_;
    this->code_ = tmp.code_;
    this->location_ = tmp.location_;
    return *this;
}

ExceptionCode::~ExceptionCode() throw() {}
const char *ExceptionCode::what() const throw()
{
    return "Message";
}

void ExceptionCode::handleException()
{
    Server &server = main_server.sp_.getServer(this->con_.reqeust_.header_["Host"], this->con_.port_);
    Response &res = this->con_.response_;
    Location &location = server.findLocation(con_.reqeust_);

    res.header_["Server"] = server.server_name_;
    res.header_["Date"] = generateTime();
    res.header_["Connection"] = "Keep-Alive";
    res.status_ = ResponseStatus(code_);
    res.http_version_ = "HTTP/1.1";

    if (code_ == 404)
    {
        res.header_["Content-Type"] = "text/html";
        res.header_["Content-Length"] = location.getFileSize(server.error_page_);
        res.file_path_ = location.getFilePath(server.error_page_);
        res.send(con_.socket_);
        this->con_.resetData();
    }
    else if (code_ == 302)
    {
        res.header_["Location"] = this->location_;
        res.writeStartLine(con_.socket_);
        res.writeHeader(con_.socket_);
        this->con_.resetData();
    }
}
