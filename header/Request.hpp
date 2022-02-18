#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "server.hpp"

class Request
{
	public:
		Request(void);
		~Request(void);

		void		setMethod(std::string method);
		void		setUrl(std::string url);
		void		setVersion(std::string version);
		void		setHeader(std::string header);
		std::string	getMethod(void);
		std::string	getUrl(void);
		std::string	getVersion(void);
		std::string	getHeader(void);
	private:
		std::string	method_;
		std::string	url_;
		std::string	version_;
		std::string	header_;
};

#endif
