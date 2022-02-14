#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include <string.h>
#include <iostream>

class Location
{
    public:
        std::string                 method;
        std::string                 redirection;
        std::string                 root;
        bool                        directoryListing;
        std::string                 dlDefault;
        std::string                 cgiExtension;
        std::string                 uploadPath;
        Location();
        ~Location();
        void dataSetting(std::string data);

    private:
};

#endif