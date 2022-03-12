#ifndef EXCEPTIONCODE_HPP
#define EXCEPTIONCODE_HPP

#include "Total.hpp"
#include "Connection.hpp"

class ExceptionCode : public std::exception
{
    public:
        ExceptionCode();
        ExceptionCode(int code);
        virtual ~ExceptionCode() throw();
        virtual const char* what() const throw();
        int getCode() const;

        Connection con;

    private:
        int _code;
        
        //int _socket;
};
#endif