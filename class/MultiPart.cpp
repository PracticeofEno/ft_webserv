#include "MultiPart.hpp"
#include "Total.hpp"

MultiPart::MultiPart(std::string data, std::string boundary)
{
    _data = data;
    _boundary = boundary;

}

MultiPart::~MultiPart() {}

void MultiPart::makeEntity()
{
    std::string tmp;
    std::string start_boundary = "--" + _boundary;
    std::string end_bondary = start_boundary + "--";

    while (true)
    {
        tmp = this->readLine();
        if (tmp.compare(end_bondary) != 0)
        {
            if (_data.compare("") == 0)
            {
                this->entity_.clear();
                break;
            }
            if (tmp.compare(start_boundary) == 0)
            {
                this->entity_.push_back(makeUploadData());
            }
        }
        else
        {
            break;
        }
    }
}

UploadData MultiPart::makeUploadData()
{
    std::string tmp;

    while (true)
    {
        tmp = this->readLine();
        //if (tmp == "\r\n")

        
    }
}

std::string MultiPart::readLine()
{
    return "";
}

