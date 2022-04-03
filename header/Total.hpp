#ifndef TOTAL_HPP
#define TOTAL_HPP

#define EPOLL_SIZE 50 // 실제론 사용되지 않음
#define BUF_SIZE 4096

#define SERVER 1
#define CLIENT 2
#define CGI 3

#define NOTEXIST -1
#define EXIST -2

// Response Ready not ready
#define NOT_READY 0
#define READY 1

//location define
#define REGULAR 0
#define DIRECTORY 1

#define ALL 100

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <map>
#include <sstream>
#include <ctime>
#include <sys/stat.h>
typedef std::basic_string<unsigned char> string;
extern std::map<std::string, std::string> mime;
#endif