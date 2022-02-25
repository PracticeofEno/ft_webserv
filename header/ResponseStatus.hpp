#ifndef RESPONSESTATUS_HPP
#define RESPONSESTATUS_HPP

#include "Total.hpp"

class ResponseStatus 
{
    public:
        std::string code_;
        std::string messasge_;

        ResponseStatus();
        ResponseStatus(int code);
        ~ResponseStatus();
        ResponseStatus(const ResponseStatus& tmp);
        ResponseStatus& operator=(const ResponseStatus& tmp);
        

    private:
        std::map<std::string, std::string> _reserve;
        void setReserve();
        void setCode(std::string code);

};
#endif