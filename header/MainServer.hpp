#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "Total.hpp"
#include "ServerPool.hpp"
#include "ConnectionPool.hpp"
#include "Location.hpp"
#include "ExceptionCode.hpp"
#include "Response.hpp"
#include <sys/time.h>

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
        bool init();
        void start();

    private:
        struct      epoll_event *_ep_events_buf; // 변화를 담을 버퍼의 주소!
        int         _epfd;
        void        makeServerPool(std::string data);
        Server      makeServer(std::string& data);
        Location    makeLocation(std::string& data, std::string name);
        void        makeMimeType(std::string data);
        void        handleReadEvent(int event_fd);
        void        handleWriteEvent(int event_fd);
        int         eventWait();

        bool        openSocket(int port);
        bool        checkValidConfig();

};
extern MainServer main_server;

#endif