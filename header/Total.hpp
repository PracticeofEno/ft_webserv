#ifndef TOTAL_HPP
#define TOTAL_HPP

#define EPOLL_SIZE 50 // 실제론 사용되지 않음
#define BUF_SIZE 4096
#define SERVER 1
#define CLIENT 2
#define NO -1

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

#endif