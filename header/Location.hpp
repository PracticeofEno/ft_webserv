#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Total.hpp"
#include "Util.hpp"

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
        std::string                 location_name_;

        /*                      OCCF                             */
        Location();
        ~Location();
        Location(const Location& location);
        Location& operator=(const Location& location);
        //////////////////////////////////////////////////////////

        void dataSetting(std::string data);
        bool methodCheck(std::string method);
        bool redirectionCheck();
        bool existFile(std::string url);
        bool isDir(std::string url);
        std::string getFilePath(std::string url);
        std::string getFileSize(std::string url);
        std::string getRecentTime(std::string url);
        std::string getDirectoryList(std::string url);
        std::string getDirectoryDefaultFile();
    private:
};

#endif