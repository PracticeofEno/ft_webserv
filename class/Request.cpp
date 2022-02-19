#include "Request.hpp"

Request::Request(void) {}

Request::~Request(void) {}

void		Request::setMethod(std::string method)
{
	method_ = method;
}

void		Request::setUrl(std::string url)
{
	url_ = url;
}

void		Request::setVersion(std::string version)
{
	version_ = version;
}

void		Request::setHeader(std::string header)
{
	header_ = header;
}

std::string	Request::getMethod(void)
{
	return method_;
}

std::string	Request::getUrl(void)
{
	return url_;
}

std::string	Request::getVersion(void)
{
	return version_;
}

std::string	Request::getHeader(void)
{
	return header_;
}
