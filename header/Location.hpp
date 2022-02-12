#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "total.hpp"

class Location
{
    public:
        std::vector<std::string>    methods;
        std::string                 redirection;
        std::string                 root;
        bool                        directoryListing;
        std::string                 cgiExtension;
        std::string                 uploadPath;

    private:
};

#endif