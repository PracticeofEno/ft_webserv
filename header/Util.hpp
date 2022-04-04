#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>

std::string &ft_ltrim(std::string &s, const char *t);
std::string &ft_rtrim(std::string &s, const char *t);
std::string &ft_trim(std::string &s, const char *t);
std::string replace_all(std::string &message, const std::string &pattern, const std::string &replace);
std::string searchMimeType(std::string url);
std::string generateTime();
int	ft_strcmp(const char *s1, const char *s2);
long get_time(void);
#endif