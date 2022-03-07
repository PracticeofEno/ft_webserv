#include "Location.hpp"

std::string &ltrim(std::string &s)
{
    s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
    return s;
}

std::string &rtrim(std::string &s)
{
    s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
    return s;
}

std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

Location::Location()
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
    data = trim(data);
    key = data.substr(0, data.find_first_of(' '));
    data = data.erase(0, data.find_first_of(' '));
    value = trim(data);
    if (key.compare("method") == 0)
        method_ = value;
    else if (key.compare("redirection") == 0)
        redirection_ = value;
    else if (key.compare("root") == 0)
        root_ = value;
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