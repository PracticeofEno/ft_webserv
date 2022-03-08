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
    this->socket_ = tmp.socket_;
    return *this;
}

void Response::send()
{
    writeStartLine();
    writeHeader();
    writeFile();
}

void Response::writeStartLine()
{
    write(this->socket_, this->http_version_.c_str(), this->http_version_.size());
    write(this->socket_, " ", 1);
    write(this->socket_, this->status_.code_.c_str(), this->status_.code_.size());
    write(this->socket_, " ", 1);
    write(this->socket_, this->status_.messasge_.c_str(), this->status_.messasge_.size());
    write(this->socket_, "\r\n", 2);
}

void Response::writeHeader()
{
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator its = this->header_.begin();
    std::map<std::string, std::string>::iterator ite = this->header_.end();
    for (it = its; it != ite; it++)
    {
        write(this->socket_, it->first.c_str(), it->first.size());
        write(this->socket_, ": ", 2);
        write(this->socket_, it->second.c_str(), it->second.size());
        write(this->socket_, "\r\n", 2);
    }
    write(this->socket_, "\r\n", 2);
}

void Response::writeFile()
{
    unsigned char *buffer;
    int length = 0;
    std::ifstream in(this->file_path_.c_str(), std::ifstream::binary);
    if (in.is_open())
    {
        in.seekg(0, in.end);
		length = (int)in.tellg();
		in.seekg(0, in.beg);
        buffer = new unsigned char[length];
        in.read((char*)buffer, length);
		in.close();
        write(this->socket_, (char*)buffer, length);
    }
    else
        std::cout << "Config file open fail" << std::endl;
    

}

void Response::addHeader(std::string key, std::string value)
{
    this->header_.insert(std::pair<std::string, std::string>(key, value));
}