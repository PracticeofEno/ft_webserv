#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "Total.hpp"
#include "ServerPool.hpp"
#include "ConnectionPool.hpp"
#include "Location.hpp"
#include "ExceptionCode.hpp"
#include "Response.hpp"

class MainServer
{
    public :
        ServerPool sp_;
        ConnectionPool cons_;
        std::map<int, int> pipe_to_fd_;
        
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

};
extern MainServer main_server;

#endif