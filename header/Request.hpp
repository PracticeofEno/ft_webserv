#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Total.hpp"

class Request
{
	public:
		Request(void);
		~Request(void);
		Request(const Request& tmp);
		Request& operator=(const Request& tmp);

		std::string	method_;
		std::string	url_;
		std::string	version_;
		std::map<std::string, std::string> header_;

	private:
		
};

#endif
