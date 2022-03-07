#include "Response.hpp"

Response::Response() {}
Response::~Response() {}

Response::Response(const Response& tmp) 
{
    *this = tmp;
}
Response& Response::operator= (const Response& tmp)
{
    this->file_path_ = tmp.file_path_;
    this->http_version_ = tmp.http_version_;
    this->header_ = tmp.header_;
    this->status_ = tmp.status_;
    return *this;
}

void Response::send(int fd)
{
    writeStartLine(fd);
    writeHeader(fd);
    writeFile(fd);
}

void Response::writeStartLine(int fd)
{
    write(fd, this->http_version_.c_str(), this->http_version_.size());
    write(fd, " ", 1);
    write(fd, this->status_.code_.c_str(), this->status_.code_.size());
    write(fd, " ", 1);
    write(fd, this->status_.messasge_.c_str(), this->status_.messasge_.size());
    write(fd, "\r\n", 2);
}

void Response::writeHeader(int fd)
{
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator its = this->header_.begin();
    std::map<std::string, std::string>::iterator ite = this->header_.end();
    for (it = its; it != ite; it++)
    {
        write(fd, it->first.c_str(), it->first.size());
        write(fd, ": ", 2);
        write(fd, it->second.c_str(), it->second.size());
        write(fd, "\r\n", 2);
    }
    write(fd, "\r\n", 2);
}

void Response::writeFile(int socket)
{
    (void)socket;
    /*
    unsigned char data[4096];

    std::ofstream out(this->file_path_.c_str(), std::ios::out | std::ios::binary);
    if (out.is_open())
    {
        std::string data;
        out.write(data, 4096);
    }
    else
        std::cout << "Config file open fail" << std::endl;
    (void)socket;
    //write(fd, this->payload_.c_str(), this->payload_.size());
    */
}

void Response::addHeader(std::string key, std::string value)
{
    this->header_.insert(std::pair<std::string, std::string>(key, value));
}