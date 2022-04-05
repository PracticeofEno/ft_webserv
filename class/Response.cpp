#include "Response.hpp"
#include "ExceptionCode.hpp"

Response::Response() : http_version_("HTTP/1.1"), state(NOT_READY), disconnect_(false) {}
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
    this->state = tmp.state;
    this->disconnect_ = tmp.disconnect_;
    return *this;
}

void Response::send(int fd)
{
    std::string send_message;
    int count;
    unsigned char *buffer = 0;
    int length = 0;

    send_message.append(writeStartLine());
    send_message.append(writeHeader());
    if (file_path_ != "")
    {
        std::ifstream is(file_path_.c_str(), std::ifstream::binary);
        if (is)
        {
            // seekg를 이용한 파일 크기 추출
            is.seekg(0, is.end);
            length = (int)is.tellg();
            is.seekg(0, is.beg);

            // malloc으로 메모리 할당
            length = length + send_message.size(); 
            buffer = new unsigned char[length];
            memcpy(buffer, send_message.c_str(), send_message.size());
            // read data as a block:
            is.read((char *)buffer + send_message.size(), length);
            is.close();
        }
    }
    else
    {
        std::cout << writeHeader() << std::endl;
        length = send_message.size() + response_data_.size();
        buffer = new unsigned char[length];
        memcpy(buffer, send_message.c_str(), send_message.size());
        memcpy(buffer + send_message.size(), response_data_.c_str(), response_data_.size());
    }
    count = write(fd, buffer, length);
    std::cout << "write count : " << count << std::endl;
    if (count == 0 && count == -1)
    {
        this->disconnect_ = true;
    }
    delete[] buffer;
}

std::string Response::writeStartLine()
{
    std::string start = http_version_ + " " + status_.code_ + " " + status_.messasge_ + "\r\n";
    return start;
}

std::string Response::writeHeader()
{
    std::string header;
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator its = this->header_.begin();
    std::map<std::string, std::string>::iterator ite = this->header_.end();
    for (it = its; it != ite; it++)
    {
        header.append(it->first + ": " + it->second + "\r\n");
    }
    header.append("\r\n");
    return header;
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
        // sleep(10);
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