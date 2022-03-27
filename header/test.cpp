#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

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
#define NOT_READY 1
#define READY 2

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
#ifndef SERVERPOOL_HPP
#define SERVERPOOL_HPP

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
#define NOT_READY 1
#define READY 2

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

#include "Server.hpp"

class ServerPool
{
    public :
        std::vector<Server> serverPool_;

        ServerPool();
        ~ServerPool();
        ServerPool(const ServerPool& tmp);
        ServerPool& operator=(const ServerPool& tmp);

        Server& getServer(std::string host, int port);
    private :

};
#endif
#include "ConnectionPool.hpp"
#include "Location.hpp"
#include "ExceptionCode.hpp"
#include "Response.hpp"

class MainServer
{
    public :
        ServerPool sp_;
        ConnectionPool cons_;
        
        MainServer();
        MainServer(std::string file_name);
        MainServer(const MainServer& tmp);
        MainServer& operator=(const MainServer& tmp);
        ~MainServer();
        void init();
        void start();

    private:
        struct epoll_event *_ep_events_buf; // 변화를 담을 버퍼의 주소!
        int     _event_cnt, _epfd;
        void makeServerPool(std::string data);
        Server makeServer(std::string& data);
        Location makeLocation(std::string& data);
        void    makeMimeType(std::string data);
        void handleReadEvent(int event_fd);
        void handleWriteEvent(int event_fd);
        
        void TestCode(Connection& tmp, Server server);
        bool openSocket(int port);

};
extern MainServer main_server;

#endif