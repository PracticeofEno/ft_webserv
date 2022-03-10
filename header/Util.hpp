#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>

std::string &ft_ltrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

std::string &ft_rtrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

std::string &ft_trim(std::string &s, const char *t = " \t\n\r\f\v")
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

#endif