#ifndef EXCEPTIONCODE_HPP
#define EXCEPTIONCODE_HPP

#include "Total.hpp"
#include "Connection.hpp"

class ExceptionCode : public std::exception
{
    public:
        ExceptionCode();
        ExceptionCode(const ExceptionCode& tmp);
        ExceptionCode& operator=(const ExceptionCode& tmp);
        ExceptionCode(int code, Connection& con);
        virtual ~ExceptionCode() throw();
        virtual const char* what() const throw();
        int getCode() const;

        Connection con_;
        ResponseStatus status_;

    private:
        int _code;

        //int _socket;
};
#endif
