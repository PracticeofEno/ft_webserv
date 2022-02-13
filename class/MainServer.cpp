#include "MainServer.hpp"

MainServer::MainServer(std::string fileName)
{
    std::ifstream inputFile(fileName.c_str());
    if (true == inputFile.is_open())
    {

        std::string s;
        std::string data;
        while (inputFile)
        {
            getline(inputFile, s);
            data.append(s);
            data.append("\r\n");
        }
        inputFile.close();
        std::cout << data << std::endl;
        makeServerPool(data);
    }
    else
        std::cout << "Config file open fail" << std::endl;
}

MainServer::~MainServer()
{
}

void MainServer::makeServerPool(std::string data)
{
    size_t endPos;
    std::string tmp;
    tmp = data;
    while (true)
    {
        endPos = tmp.find("\r\n");
        if (endPos != std::string::npos)
        {
            if (tmp.find("server", endPos) != std::string::npos && tmp.find("{", endPos) != std::string::npos)
            {
                tmp.erase(0, endPos + 2);
                sp.serverPool.push_back(makeServer(tmp));
            }
        }
    }
    (void)data;
}

Server &makeServer(std::string data)
{
    std::string tmp;
    Server server;
    size_t endPos;

    tmp = data;
    while (true)
    {
        endPos = tmp.find("\r\n");
        if (tmp.find("location", endPos) != std::string::npos && tmp.find("{", endPos) != std::string::npos)
        {
            Location location;
        }
    }
}