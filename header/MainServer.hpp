#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "Total.hpp"

#define EPOLL_SIZE 50 // 실제론 사용되지 않음
#define BUF_SIZE 4096
#define SERVER 1
#define CLIENT 2

class MainServer
{
    public :
        ServerPool sp_;
        ConnectionPool cons_;
        
        MainServer(std::string file_name);
        ~MainServer();
        void init();
        void start();

    private:
        struct epoll_event *_ep_events_buf; // 변화를 담을 버퍼의 주소!
        int     _event_cnt, _epfd;
        void makeServerPool(std::string data);
        Server makeServer(std::string& data);
        Location makeLocation(std::string& data);

};

#endif