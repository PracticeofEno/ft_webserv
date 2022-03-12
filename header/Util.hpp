#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>

std::string &ft_ltrim(std::string &s);
std::string &ft_rtrim(std::string &s);
std::string &ft_trim(std::string &s);
std::string replace_all(std::string &message, const std::string &pattern, const std::string &replace);
#endif