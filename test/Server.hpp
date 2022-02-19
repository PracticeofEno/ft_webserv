#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <cerrno>
// socket(), accept(), listen(), send(), recv(), bind(), connect(), inet_addr(), setsockopt(), getsockname()
#include <sys/socket.h>
// struct sockaddr_in
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

class Server
{
	public:
		Server(void);
		Server(unsigned int port);
		~Server(void);
	private:
		int					serv_sock_;
		int					clnt_sock_;
		struct sockaddr_in	serv_addr_;
		struct sockaddr_in	clnt_addr_;
		socklen_t			clnt_addr_size_;
		std::string			reqData_;
};

#endif
