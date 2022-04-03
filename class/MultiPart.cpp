#include "MultiPart.hpp"
#include "Total.hpp"

MultiPart::MultiPart(std::string body, std::string boundary)
{
    _body = body;
    _boundary = boundary;
}

MultiPart::~MultiPart() {}

void MultiPart::makeEntity()
{
    std::string buf;
    std::string start_boundary = "---" + _boundary;
    std::string end_boundary = start_boundary + "--";
    size_t index;

    while (true)
    {
        buf = readLine();
        // if (buf == start_boundary)
        if (buf != end_boundary && buf == start_boundary)
        {
            index = _body.find(start_boundary);
            this->entity_.push_back(UploadData(_body.substr(0, index)));
            _body.erase(0, index);
        }
        else if (buf == end_boundary)
        {
            break;
        }
    }
}

void MultiPart::parseEntity()
{
    std::vector<UploadData>::iterator it;
    std::vector<UploadData>::iterator its = entity_.begin();
    std::vector<UploadData>::iterator ite = entity_.end();

    for (it = its; it != ite; it++)
    {
        it->parseHeader();
    }
}

int MultiPart::writeEntity(Location& location)
{
    std::vector<UploadData>::iterator it;
    std::vector<UploadData>::iterator its = entity_.begin();
    std::vector<UploadData>::iterator ite = entity_.end();

    for (it = its; it != ite; it++)
    {
        // 헤더에서 파일 이름 들고오기
        std::string filename = it->header_["Content-Disposition"];
        size_t range_start = filename.find("filename") + sizeof("filename\"");
        size_t range_end = filename.find("\"", range_start, 1) - range_start;
        filename = "/" + filename.substr(range_start, range_end);

        int fd;
        std::string file_path = location.getUploadPath(filename);

        if ((fd = open(file_path.c_str(), O_CREAT | O_WRONLY | O_NONBLOCK)) < 0)
            std::cout << "open error occur: " << errno << std::endl;
        if (write(fd, it->body_.c_str(), it->body_.size() - 2) < 0)
            std::cout << "write error occur" << std::endl;
    }
    return (0);
}

std::string MultiPart::readLine()
{
    std::string ret("");
    size_t      index = _body.find("\r\n");
    if (index != std::string::npos)
    {
        ret = _body.substr(0, index);
        _body.erase(0, index + 2);
    }
    return (ret);
}
