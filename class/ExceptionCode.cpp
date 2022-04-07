#include "ExceptionCode.hpp"
#include "MainServer.hpp"
#include "Connection.hpp"
#include "ResponseStatus.hpp"

ExceptionCode::ExceptionCode() : code_(0) {}
ExceptionCode::ExceptionCode(int code) 
{
    status_ = ResponseStatus(code);
    code_ = code;
}
ExceptionCode::ExceptionCode(const ExceptionCode &tmp) 
{
    *this = tmp;
}

ExceptionCode &ExceptionCode::operator=(const ExceptionCode &tmp)
{
    this->status_ = tmp.status_;
    this->code_ = tmp.code_;
    this->error_str = tmp.error_str;
    this->location_ = tmp.location_;
    return *this;
}

ExceptionCode::~ExceptionCode() throw() {}
const char *ExceptionCode::what() const throw()
{
    return "Message";
}