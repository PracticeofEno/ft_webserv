#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Total.hpp"
#include "ResponseStatus.hpp"
class Connection;

#define NOT_READY 0
#define SEND 1

class Response 
{
    public:
        std::string http_version_;
        ResponseStatus status_;
        std::map<std::string, std::string> header_;
        std::string file_path_;
        std::string response_data_;
        int state;

        Response();
        ~Response();
        Response(const Response& tmp);
        Response& operator=(const Response& tmp);

        void send(int fd);
        void addHeader(std::string key, std::string value);
        void resetData();
        void readPipe(int pipe);
    private:
        
        void writeStartLine(int fd);
        void writeHeader(int fd);
        void writeHeaderCGI(int fd);
        void writeFile(int fd);

        
};
#endif