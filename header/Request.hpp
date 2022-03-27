#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Total.hpp"

#define START_LINE 0
#define HEADERS 1
#define BODY 2
#define DONE_REQUST 3

class Connection;
class request;
class Location;

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
		std::string body_;
		std::string query_;

		int getState();
		std::string readLine();
		void readSocket(int socket);
		void readPipe(int socket);
		bool parseSocket();
		void parseStartline(std::string tmp);
		void parseHeaders(std::string tmp);
		bool checkMethod(std::string method);
		bool checkUrl(std::string url);
		bool checkVersion(std::string version);
		void checkRequest(Connection& con, Request& request, Location& location);
		void resetData();

		std::string _buffer;
		std::string _buffer_cgi;
	private:

		int	state;

};

#endif
