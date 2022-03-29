#ifndef UPLOADDATA_HPP
#define UPLOADDATA_HPP

#include "Total.hpp"

class UploadData
{
    public:
        std::map<std::string, std::string> header_;
        string body_;

        UploadData();
        ~UploadData();
        UploadData(const UploadData& tmp);
        UploadData& operator=(const UploadData& tmp);

    private:
};

#endif