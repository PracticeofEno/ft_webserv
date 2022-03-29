#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Total.hpp"

#define START_LINE 0
#define HEADERS 1
#define BODY 2
#define CHUNKED 3
#define DONE_REQUST 4

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
		std::string location_;
		std::string file_;

		int getState();
		std::string readLine();
		void readSocket(int socket);
		bool parseSocket();
		void parseStartline(std::string tmp);
		void parseHeaders(std::string tmp);
		bool checkMethod(std::string method);
		bool checkUrl(std::string url);
		bool checkVersion(std::string version);
		void checkRequest(Connection& con, Request& request, Location& location);
		void resetData();
		void printStartLine();
		void setLocationFile();

		std::string _buffer;
	private:

		int	state;

};

#endif
