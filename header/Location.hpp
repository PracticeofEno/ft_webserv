#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include <string.h>
#include <iostream>

class Location
{
    public:
        std::string                 method_;
        std::string                 redirection_;
        std::string                 root_;
        bool                        directory_listing_;
        std::string                 dl_default_;
        std::string                 cgi_extension_;
        std::string                 upload_path_;

        /*                      OCCF                             */
        Location();
        ~Location();
        Location(const Location& location);
        Location& operator=(const Location& location);
        //////////////////////////////////////////////////////////

        void dataSetting(std::string data);

    private:
};

#endif