#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "total.hpp"

class MainServer
{
    public :
        ServerPool sp;
        
        MainServer(std::string fileName);
        ~MainServer();

    private:
        void makeServerPool(std::string data);
        Server* makeServer(std::string& data);
        Location* makeLocation(std::string& data);

};

#endif