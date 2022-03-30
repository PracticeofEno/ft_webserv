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
        ExceptionCode(int code);
        ExceptionCode(int code, Connection& con);
        virtual ~ExceptionCode() throw();
        virtual const char* what() const throw();

        Connection& con_;
        ResponseStatus status_;
        std::string location_;
        std::string error_str;
        int code_;
        void handleException();

    private:
        


        //int _socket;
};
#endif
