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
        std::string file_path_;
        int socket_;

        Response();
        ~Response();
        Response(const Response& tmp);
        Response& operator=(const Response& tmp);

        void send();
        void addHeader(std::string key, std::string value);
    private:
        void writeStartLine();
        void writeHeader();
        void writeFile();
};
#endif