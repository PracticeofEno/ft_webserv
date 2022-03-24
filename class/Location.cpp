#include "Location.hpp"
#include "ExceptionCode.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

Location::Location() : redirection_("")
{
}

Location::~Location()
{
}

Location::Location(const Location& location)
{
    *this = location;
}

Location& Location::operator=(const Location& location)
{
    this->cgi_extension_ = location.cgi_extension_;
    this->dl_default_ = location.dl_default_;
    this->root_ = location.root_;
    this->method_ = location.method_;
    this->upload_path_ = location.upload_path_;
    this->directory_listing_ = location.directory_listing_;
    this->redirection_ = location.redirection_;
    return *this;
}

void Location::dataSetting(std::string data)
{
    std::string key, value;
    data = ft_trim(data, " \t\n\r\f\v");
    key = data.substr(0, data.find_first_of(' '));
    data = data.erase(0, data.find_first_of(' '));
    value = ft_trim(data, " \t\n\r\f\v");
    if (key.compare("method") == 0)
        method_ = value;
    else if (key.compare("redirection") == 0)
        redirection_ = value;
    else if (key.compare("root") == 0)
        root_ = ft_rtrim(value, "/");
    else if (key.compare("cgiExtension") == 0)
        cgi_extension_ = value;
    else if (key.compare("uploadPath") == 0)
        upload_path_ = value;
    else if (key.compare("dlDefault") == 0)
        dl_default_ = value;
    else if (key.compare("directoryListing") == 0)
    {
        directory_listing_ = false;
        if (value.compare("on") == 0)
            directory_listing_ = true;
        else if (value.compare("off") == 0)
            directory_listing_ = false;
    }
}

bool Location::methodCheck(std::string method)
{
    if (this->method_.find(method) != std::string::npos)
        return (true);
    else
        return (false);
}

bool Location::redirectionCheck()
{
    if (this->redirection_ == "")
        return false;
    return true;
}

bool Location::existFile(std::string url)
{
    std::string tmp;
    tmp = getFilePath(url);

    if (access(tmp.c_str(), F_OK) == 0)
        return true;
    else
        return false;
}

std::string Location::getFilePath(std::string url)
{
    char buf[4096];
    realpath(".", buf);
    std::string current_path(buf);
    current_path.append(this->root_);
    current_path.append(url);
    current_path = replace_all(current_path, "//", "/");
    return current_path;
}

std::string Location::getFileSize(std::string url)
{
    std::string path;
    struct stat st;
    std::stringstream ss;

    path = getFilePath(url);
    lstat(path.c_str(), &st);
    ss << st.st_size;
    return (ss.str());
}

std::string Location::getRecentTime(std::string url)
{
    std::string path;
    std::string ret;
    struct stat st;

    path = getFilePath(url);
    lstat(path.c_str(), &st);
    ret = std::string(ctime(&st.st_mtime));
    ret.erase(ret.find("\n"), 1);
    return (ret);
}

bool Location::isDir(std::string url)
{
    struct stat sb;

    if (stat(this->getFilePath(url).c_str(), &sb) == -1)
    {
        std::cout << "Stat Error" << std::endl;
        //throw ExceptionCode(999);
    }
    if (sb.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

std::string Location::getDirectoryList(std::string url)
{
    std::string buf;
    std::string file_list;
    DIR *dir_ptr = NULL;
    struct dirent *file = NULL;
    

    if ((dir_ptr = opendir(getFilePath(url).c_str())) == NULL)
    {
        std::cout << "opendir error" << std::endl;
        //throw ExceptionCode(999);
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

    return buf;
}

std::string Location::getDirectoryDefaultFile()
{
    char buf[4096];
    realpath(".", buf);
    std::string current_path(buf);
    current_path.append("/www/");
    current_path.append(dl_default_);
    current_path = replace_all(current_path, "//", "/");
    return current_path;
}