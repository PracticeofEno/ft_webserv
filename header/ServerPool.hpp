#ifndef SERVERPOOL_HPP
#define SERVERPOOL_HPP

class Server;

class ServerPool
{
    public :
        std::vector<Server> serverPool;

        ServerPool();
        ~ServerPool();
    private :

};
#endif