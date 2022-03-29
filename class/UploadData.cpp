#include "UploadData.hpp"

UploadData::UploadData() {}
UploadData::~UploadData() {}
UploadData::UploadData(const UploadData& tmp) 
{
    *this = tmp;
}
UploadData& UploadData::operator=(const UploadData& tmp)
{
    this->body_ = tmp.body_;
    this->header_ = tmp.header_;
    return *this;
}