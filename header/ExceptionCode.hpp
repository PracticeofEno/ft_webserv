#ifndef EXCEPTIONCODE_HPP
#define EXCEPTIONCODE_HPP

#include "Total.hpp"
#include "ResponseStatus.hpp"

class Connection;

class ExceptionCode : public std::exception
{
    public:
        ExceptionCode();
        ExceptionCode(const ExceptionCode& tmp);
        ExceptionCode& operator=(const ExceptionCode& tmp);
        ExceptionCode(int code);
        virtual ~ExceptionCode() throw();
        virtual const char* what() const throw();

        ResponseStatus status_;
        std::string location_;
        std::string error_str;
        int code_;

    private:
        


        //int _socket;
};
#endif
