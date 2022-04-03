#include "UploadData.hpp"

UploadData::UploadData(){}
UploadData::~UploadData() {}
UploadData::UploadData(const UploadData& tmp) 
{
    *this = tmp;
}
UploadData& UploadData::operator=(const UploadData& tmp)
{
    this->body_ = tmp.body_;
    this->header_ = tmp.header_;
    return *this;
}

UploadData::UploadData(std::string body)
{
    body_ = body;
}

void UploadData::parseHeader()
{
    std::string buf;
    size_t      pos;
    std::string key;
    std::string value;


    while (true)
    {
        buf = readLine();
        if (buf.size() > 0)
        {
            pos = buf.find(": ");
            key = buf.substr(0, pos);
            buf.erase(0, pos + 2);
            value = buf;
            header_.insert(std::pair<std::string, std::string>(key, value));
        }
        else
        {
            break;
        }
    }
}

std::string UploadData::readLine()
{
    std::string ret("");
    size_t      index = body_.find("\r\n");
    if (index != std::string::npos)
    {
        ret = body_.substr(0, index);
        body_.erase(0, index + 2);
    }
    return (ret);
}
