#include "Response.hpp"
#include "ExceptionCode.hpp"

Response::Response() : http_version_("HTTP/1.1"), state(NOT_READY) {}
Response::~Response() {}

Response::Response(const Response &tmp)
{
    *this = tmp;
}
Response &Response::operator=(const Response &tmp)
{
    this->file_path_ = tmp.file_path_;
    this->http_version_ = tmp.http_version_;
    this->header_ = tmp.header_;
    this->status_ = tmp.status_;
    this->response_data_ = tmp.response_data_;
    return *this;
}

void Response::send(int fd)
{
    writeStartLine(fd);
    writeHeader(fd);
    if (file_path_ != "")
        writeFile(fd);
    else
        write(fd, response_data_.c_str(), response_data_.size());
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

void Response::writeHeaderCGI(int fd)
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
    // write(fd, "\r\n", 2);
}

void Response::writeFile(int fd)
{
    std::ifstream is(file_path_.c_str(), std::ifstream::binary);
    if (is)
    {
        // seekg를 이용한 파일 크기 추출
        is.seekg(0, is.end);
        int length = (int)is.tellg();
        is.seekg(0, is.beg);

        // malloc으로 메모리 할당
        unsigned char *buffer = new unsigned char[length];
        // read data as a block:
        is.read((char *)buffer, length);
        is.close();
        write(fd, buffer, length);
        delete[] buffer;
    }
}

void Response::addHeader(std::string key, std::string value)
{
    this->header_.insert(std::pair<std::string, std::string>(key, value));
}

void Response::resetData()
{
    http_version_.clear();
    this->status_ = ResponseStatus();
    this->header_.clear();
    this->file_path_.clear();
    this->response_data_.clear();
    this->state = NOT_READY;
}

void Response::readPipe(int pipe)
{
    int tmp = 4000;
    char buf[tmp];
    int strlen;
    while (42)
    {
        strlen = read(pipe, buf, tmp);
        buf[strlen] = 0;
        if (strlen == 0)
        {
            std::cout << "pipe has been empty!" << std::endl;
            break;
        }
        else if (strlen > 0)
        {
            this->response_data_.append(buf);
        }
        else
            break;
        // 음수일 때 에러 처리 필요할까?
    }
}