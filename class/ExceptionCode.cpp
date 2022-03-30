#include "ExceptionCode.hpp"
#include "MainServer.hpp"

ExceptionCode::ExceptionCode() : con_(main_server.cons_.cons_[0]) {}
ExceptionCode::ExceptionCode(int code) : con_(main_server.cons_.cons_[0])
{
    status_ = ResponseStatus(code);
    code_ = code;
}
ExceptionCode::ExceptionCode(int code, Connection &con) : con_(con), code_(code)
{
    status_ = ResponseStatus(code);
    con_ = con;
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
    this->error_str = tmp.error_str;
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
        res.header_["Content-Length"] = location.getFileSize("/" + server.error_page_);
        res.file_path_ = location.getFilePath("/" + server.error_page_);
        res.send(con_.socket_);
    }
    else if (code_ == 400)
    { 
        res.response_data_ = this->error_str;
        std::stringstream data_size;
        data_size << res.response_data_.size();
        //res.header_["Content-Type"] = "text/html";
        res.header_["Content-Length"] = data_size.str();
        res.send(con_.socket_);
    }
    else if (code_ == 405)
    {
        //res.header_["Content-Length"] = 
        //res.header_
        //res.send(con_.socket_);
    }
    //else if (code_ == 500)
}
