#include "Server.hpp"

Server::Server(void) {}

Server::Server(unsigned int port) : clnt_addr_size_(sizeof(serv_addr_))
{
	serv_sock_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // ipv4, tcp
	if (serv_sock_ == -1)
	{
		std::cerr << "socket error!" << std::endl;
		_exit(1);
	}

	// server fd Non-Blocking Socket으로 설정. Edge Trigger 사용하기 위해 설정.
	int	serv_flag = fcntl(serv_sock_, F_GETFL, 0); // 파일의 속성을 가져온다.
	fcntl(serv_sock_, F_SETFL, serv_flag | O_NONBLOCK); // 기존 속성에 NONBLOCK 옵션을 추가한다.

	// 강제종료나 비정상 종료가 된 경우에 bind 된 데이터를 재사용 할 수 있도록 한다.
	int on = 0;
	if(setsockopt(serv_sock_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		std::cerr << "socket option set error!" << std::endl;
		_exit(1);
	}

	memset(&serv_addr_, 0, sizeof(serv_addr_));

	serv_addr_.sin_family = PF_INET; // ipv4
	serv_addr_.sin_addr.s_addr = INADDR_ANY; // ip address
	serv_addr_.sin_port = htons(port); // port

	// dummy data는 필수적으로 0으로 초기화 되어 있어야 한다.
	memset(serv_addr_.sin_zero, 0, sizeof(serv_addr_.sin_zero));

	// 서버의 주소와 소켓 디스크립터를 결합한다.
	if (bind(serv_sock_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_)) == -1)
	{
		std::cerr << "bind error!" << std::endl;
		_exit(1);
	}

	// 연결 대기열을 생성한다.
	if (listen(serv_sock_, 10) == -1)
	{
		std::cerr << "listen error!" << std::endl;
		_exit(1);
	}

	int	epoll_fd = epoll_create(1024);
	if (epoll_fd < 0)
	{
		std::cerr << "epoll create error!" << std::endl;
		close(serv_sock_);
		_exit(1);
	}

	struct epoll_event	events;
	events.events = EPOLLIN | EPOLLET;
	events.data.fd = serv_sock_;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock_, &events) < 0)
	{
		std::cerr << "epoll control error!" << std::endl;
		close(serv_sock_);
		close(epoll_fd);
		_exit(1);
	}

	int					MAX_EVENTS = 1024;
	struct epoll_event	epoll_events[MAX_EVENTS];
	int					event_count;
	int					timeout = -1;

	char	buf[1024] = {0};
	int		read_val;

	while (1)
	{
		event_count = epoll_wait(epoll_fd, epoll_events, MAX_EVENTS, timeout);

		if (event_count < 0)
		{
			std::cerr << "epoll wait error!" << std::endl;
			break ;
		}

		for (int i = 0; i < event_count; i++)
		{
			if (epoll_events[i].data.fd == serv_sock_)
			{
				std::cout << "client accept" << std::endl;

				// 제일 처음 연결된 연결을 가져와서 새로운 연결된 소켓을 만든다.
				clnt_sock_ = accept(serv_sock_, (struct sockaddr*)&clnt_addr_, (socklen_t*)&clnt_addr_size_);
				if (clnt_sock_ == -1)
				{
					std::cerr << "accept error!" << std::endl;
					_exit(1);
				}

				int	clnt_flag = fcntl(clnt_sock_, F_GETFL, 0); // 파일의 속성을 가져온다.
				fcntl(clnt_sock_, F_SETFL, clnt_flag | O_NONBLOCK); // 기존 속성에 NONBLOCK 옵션을 추가한다.

				struct epoll_event	events;
				events.events = EPOLLIN | EPOLLET;
				events.data.fd = clnt_sock_;

				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clnt_sock_, &events) < 0)
				{
					std::cerr << "client epoll control error!" << std::endl;
					close(clnt_sock_);
					continue ;
				}
			}
			else
			{
				read_val = read(clnt_sock_, buf, sizeof(buf));

				if (read_val == 0)
				{
					std::cout << "client socket close!" << std::endl;
					close(clnt_sock_);
				}
				else if (read_val < 0)
				{
					// 일반적으로 read 함수는 유용한 입력 없을 시 기다리는데 non-block 모드에서는 즉시 반환 후 에러를 보고한다.
					if (errno == EAGAIN)
						std::cout << "read EAGAIN" << std::endl;
					else
					{
						std::cerr << "read error!" << std::endl;
						std::cout << "client socket close!" << std::endl;
						close(clnt_sock_);
						break ;
					}
				}
				std::cout << buf << std::endl;
				char	conMsg[] = "Connected to server!\n";
				write(clnt_sock_, conMsg, strlen(conMsg));
			}
		}
		std::cout << "Waiting for new connection" << std::endl;
	}
}

Server::~Server(void) {}
