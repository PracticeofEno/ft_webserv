#include "MainsServer.hpp"

MainServer::MainServer(std::string fileName)
{
    std::ifstream inputFile(fileName.c_str());
    if (true == inputFile.is_open()) {

        std::string s;
        std::string data;
        while (inputFile) 
        {
            getline(inputFile, s);
            data.append(s);
        }  
        inputFile.close();
    } 
    else 
        std::cout << "Config file open fail" << std::endl;
}

MainServer::~MainServer()
{

}

/*
Server MainServer::makeServer(std::ifstream& inputFile)
{
    std::string s;
    std::string data;
}
*/