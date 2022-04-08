#include "Connection.hpp"
#include "MainServer.hpp"
#include "unistd.h"


Connection::Connection() {}
Connection::Connection(int socket, int kind, int port) : socket_(socket), kind_(kind), timeout_(20000), port_(port), pipe_read_(0) {}

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
    this->port_ = tmp.port_;
    this->reqeust_ = tmp.reqeust_;
    this->response_ = tmp.response_;
    this->client_ip_ = tmp.client_ip_;
    this->disconnect_ = tmp.disconnect_;
    this->e_ = tmp.e_;
    return *this;
}

bool Connection::makeRequest()
{
    this->reqeust_.readSocket(this->socket_);
    // std::cout << "==== buf content =====" << std::endl;
    // std::cout << this->reqeust_._buffer << std::endl;
    // std::cout << "========================================" << std::endl;
    this->reqeust_.parseSocket();
    return true;
}

void Connection::resetData()
{
    this->kind_ = CLIENT;
    this->e_.code_ = 0;
    response_.resetData();
    reqeust_.resetData();
}
void Connection::handleException()
{
    if (this->e_.code_ == 999)
        return;

    Server& server = main_server.sp_.getServer(this->reqeust_.header_["Host"], this->port_);
    Response& res = this->response_;
    Location& location = server.findLocation(this->reqeust_);

    res.header_["Server"] = server.server_name_;
    res.header_["Date"] = generateTime();
    res.header_["Connection"] = "Keep-Alive";
    res.header_["Keep-Alive"] = "20";
    res.status_ = ResponseStatus(this->e_.code_);
    res.http_version_ = "HTTP/1.1";

    if (this->e_.code_ == 404)
    {
        res.header_["Content-Type"] = "text/html";
        res.header_["Content-Length"] = location.getFileSize("/" + server.error_page_);
        res.file_path_ = location.getServerRootPath("/" + server.error_page_);
        res.send(this->socket_);
        this->resetData();
    }
    else if (this->e_.code_ == 400)
    { 
        res.response_data_ = this->e_.error_str;
        std::stringstream data_size;
        data_size << res.response_data_.size();
        res.header_["Content-Length"] = data_size.str();
        res.file_path_ = "";
        res.send(this->socket_);
        this->resetData();
    }
    else if (this->e_.code_ == 405)
    {
        res.response_data_ = "Method Not Allowed\n";
        res.header_["Allow"] = location.method_;
        res.header_["Content-Length"] = "18";
        res.send(this->socket_);
        this->resetData();
    }
    else if (this->e_.code_ == 413)
    {
        std::stringstream size;
        res.response_data_ = "Request Entity Too Large\n";
        size << server.client_body_size_;
        res.header_["clinet_body_size"] =  size.str();
        res.header_["Content-Length"] = "24";
        res.send(this->socket_);
        this->resetData();
    }
    else if (this->e_.code_ == 302)
    {
        res.header_["Location"] = this->e_.location_;
        std::cout << "Location header is " << this->e_.location_ << std::endl;
        res.send(this->socket_);
        this->resetData();
    }
}