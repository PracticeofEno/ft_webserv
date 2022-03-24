#include "ExceptionCode.hpp"

ExceptionCode::ExceptionCode() {}
ExceptionCode::ExceptionCode(int code) 
{
    status_ = ResponseStatus(code);
    _code = code;
}
ExceptionCode::ExceptionCode(int code, Connection& con) : _code(code) 
{
    status_ = ResponseStatus(code);
    con_ = con;
}
ExceptionCode::ExceptionCode(const ExceptionCode& tmp)
{
    *this = tmp;
}

ExceptionCode& ExceptionCode::operator=(const ExceptionCode& tmp)
{
    this->con_ = tmp.con_;
    this->status_ = tmp.status_;
    this->_code = tmp._code;
    return *this;
}

ExceptionCode::~ExceptionCode() throw() {}
const char* ExceptionCode::what() const throw()
{
    return "Message";
}

int ExceptionCode::getCode() const
{
    return _code;
}
