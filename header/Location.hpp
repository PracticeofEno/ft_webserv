#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include <string.h>
#include <iostream>

class Location
{
    public:
        std::vector<std::string>    methods;
        std::string                 redirection;
        std::string                 root;
        bool                        directoryListing;
        std::string                 cgiExtension;
        std::string                 uploadPath;
        Location();
        ~Location();

    private:
};

#endif