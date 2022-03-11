#include "Request.hpp"

Request::Request(void) {}

Request::~Request(void) {}

Request::Request(const Request& tmp)
{
    *this = tmp;
}
    
Request& Request::operator= (const Request& tmp)
{
    this->url_ = tmp.url_;
    this->method_ = tmp.method_;
    this->header_ = tmp.header_;
    this->version_ = tmp.version_;
    this->query_ = tmp.query_;
    return *this;
}