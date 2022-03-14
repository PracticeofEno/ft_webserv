#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Total.hpp"

class Request
{
	public:
		Request();
		~Request();
		Request(const Request& tmp);
		Request& operator=(const Request& tmp);

		std::string	method_;
		std::string	url_;
		std::string	version_;
		std::map<std::string, std::string> header_;
		std::string query_;
		
		int getState();
		std::string readLine();
		void readSocket(int socket);
		void resetData();

		std::string _buffer;
	private:
		
		int	state;
		
};

#endif
