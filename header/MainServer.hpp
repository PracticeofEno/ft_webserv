#ifndef MAINSERVER_HPP
#define MAINSERVER_HPP

#include "Total.hpp"

class MainServer
{
    public :
        ServerPool sp_;
        
        MainServer(std::string file_name);
        ~MainServer();
        void init();

    private:
        void makeServerPool(std::string data);
        Server makeServer(std::string& data);
        Location makeLocation(std::string& data);

};

#endif