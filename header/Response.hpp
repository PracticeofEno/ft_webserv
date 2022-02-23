#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Total.hpp"
#include "ResponseStatus.hpp"

class Response 
{
    public:
        std::string http_version_;
        ResponseStatus status_;
        std::string status_code_;
        std::string status_message_;
        std::map<std::string, std::string> header_;
        std::string payload_;

        Response();
        ~Response();

        void send(int fd);
    private:

};
#endif