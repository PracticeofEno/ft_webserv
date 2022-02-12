#include "MainsServer.hpp"

MainServer::MainServer(std::string fileName)
{
    std::ifstream inputFile(fileName.c_str());
    if (true == inputFile.is_open()) {
        std::string s;
        while (inputFile) {
            getline(inputFile, s);  
            std::cout << s << std::endl;
        }  
        inputFile.close();
    } else {
        std::cout << "Config file open fail" << std::endl;
    }
}

MainServer::~MainServer()
{

}