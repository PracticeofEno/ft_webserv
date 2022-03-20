#include "Response.hpp"
#include "ExceptionCode.hpp"
#include <sys/types.h>
#include <dirent.h>

Response::Response() {}
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
    this->file_data_ = tmp.file_data_;
    return *this;
}

void Response::send(int fd)
{
    struct stat sb;
    if (stat(file_path_.c_str(), &sb) == -1)
    {
        std::cout << "Stat Error" << std::endl;
        throw ExceptionCode(999);
    }
    writeStartLine(fd);
    if (sb.st_mode & S_IFDIR)
    {
        writeDirectory(fd);
    }
    else
    {
        writeHeader(fd);
        writeFile(fd);
    }
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
    this->file_data_.clear();
}

void Response::writeDirectory(int fd)
{
    std::string buf;
    std::string file_list;
    DIR *dir_ptr = NULL;
    struct dirent *file = NULL;
    std::stringstream ss;

    if ((dir_ptr = opendir(file_path_.c_str())) == NULL)
    {
        std::cout << "opendir error" << std::endl;
        throw ExceptionCode(999);
    }
    while ((file = readdir(dir_ptr)) != NULL)
    {
        file_list.append("<a href=\"./");
        file_list.append(file->d_name);
        file_list.append("\">");
        file_list.append(file->d_name);
        file_list.append("</a><br>\r\n");
    }
    buf.append("<!DOCTYPE html>\r\n");
    buf.append("<html>\r\n");
    buf.append("<head>\r\n");
    buf.append("</head>\r\n");
    buf.append("<body>\r\n");
    buf.append(file_list);
    buf.append("</body>\r\n");
    buf.append("</html>\r\n");

    ss << buf.size();
    header_["Content-Type"] = "text/html";
    header_["Content-Length"] = ss.str();
    writeHeader(fd);
    write(fd, buf.c_str(), buf.size());
}