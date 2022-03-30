#ifndef UPLOADDATA_HPP
#define UPLOADDATA_HPP

#include "Total.hpp"

class UploadData
{
    public:
        std::map<std::string, std::string> header_;
        std::string body_;

        UploadData();
        ~UploadData();
        UploadData(const UploadData& tmp);
        UploadData& operator=(const UploadData& tmp);

        UploadData(std::string body);
        void parseHeader();
        std::string readLine();
    private:
};

#endif