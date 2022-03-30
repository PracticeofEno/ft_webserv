#ifndef MULTIPART_HPP
#define MULTIPART_HPP

#include "Total.hpp"
#include "UploadData.hpp"
#include "Location.hpp"

#define HEADER 1

class MultiPart 
{
    public:
        std::vector<UploadData> entity_;

        MultiPart(std::string body, std::string boundary);
        ~MultiPart();

        void    makeEntity();
        void    parseEntity();
        int     writeEntity(Location& location);

    private:
        std::string _body;
        std::string _boundary;

        std::string readLine();
};

#endif
