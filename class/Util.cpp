#include "Util.hpp"
#include "Total.hpp"
#define MIMETYPE std::map<std::string, std::string>

std::string &ft_ltrim(std::string &s, const char *t)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

std::string &ft_rtrim(std::string &s, const char *t)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

std::string &ft_trim(std::string &s, const char *t)
{
    return ft_ltrim(ft_rtrim(s, t), t);
}

std::string replace_all(std::string &message, const std::string &pattern, const std::string &replace)
{
    std::string result = message;
    std::string::size_type pos = 0;
    while ((pos = result.find(pattern)) != std::string::npos)
    {
        result.replace(pos, pattern.size(), replace);
    }
    return result;
}

std::string searchMimeType(std::string url)
{
    std::string ret = "application/octet-stream";
    size_t index = url.find(".");
    MIMETYPE::iterator it;
    MIMETYPE::iterator its = mime.begin();
    MIMETYPE::iterator ite = mime.end();
    if (index != std::string::npos)
    {
        std::string ext = url.substr(index + 1, std::string::npos);
        if (ext.compare("") != 0)
        {
            for (it = its; it != ite; it++)
            {
                if (it->first.compare(ext) == 0)
                {
                    ret = it->second;
                    break;
                }
            }
        }
    }
    return ret;
}

std::string generateTime()
{
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    return (std::string(buf));
}