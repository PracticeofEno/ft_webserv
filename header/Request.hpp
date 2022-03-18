#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Total.hpp"
// #include "ExceptionCode.hpp"

#define FILL_START_LINE 1
#define DONE_REQUEST 2
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
		bool parseSocket();
		bool checkMethod(std::string method);
		void resetData();

		std::string _buffer;
	private:

		int	state;

};

#endif
