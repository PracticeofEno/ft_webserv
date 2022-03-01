#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

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