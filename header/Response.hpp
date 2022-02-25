#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Total.hpp"
#include "ResponseStatus.hpp"

class Response 
{
    public:
        std::string http_version_;
        ResponseStatus status_;
        std::map<std::string, std::string> header_;
        std::string payload_;

        Response();
        ~Response();

        void send(int fd);
        void addHeader(std::string key, std::string value);
    private:
        void writeStartLine(int fd);
        void writeHeader(int fd);
        void writePayload(int fd);
};
#endif