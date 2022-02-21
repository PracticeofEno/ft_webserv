#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Total.hpp"

class Response 
{
    public:
        std::string method_;
        int         status_code_;
        std::string status_message;
        std::map<std::string, std::string> header;
        std::string payload;

        Response();
        ~Response();


    private:

};

#endif