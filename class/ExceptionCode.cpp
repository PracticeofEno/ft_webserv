#include "ExceptionCode.hpp"

ExceptionCode::ExceptionCode() {}
ExceptionCode::ExceptionCode(int code) : _code(code) {}
ExceptionCode::~ExceptionCode() throw() {}
const char* ExceptionCode::what() const throw()
{
    return "Message";
}

int ExceptionCode::getCode() const
{
    return _code;
}
