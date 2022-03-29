#ifndef MULTIPART_HPP
#define MULTIPART_HPP

#include "Total.hpp"
#include "UploadData.hpp"

class MultiPart 
{
    public:
        std::vector<UploadData> entity_;
        MultiPart(std::string data, std::string boundary);
        ~MultiPart();

    private:
        std::string _data;
        std::string _boundary;

        void makeEntity();
        std::string readLine();
        UploadData makeUploadData();
};


#endif