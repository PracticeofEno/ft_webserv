#include "MainServer.hpp"

MainServer::MainServer()
{
}

MainServer::~MainServer()
{
}

MainServer::MainServer(const MainServer &tmp)
{
    *this = tmp;
}

MainServer &MainServer::operator=(const MainServer &tmp)
{
    this->sp_ = tmp.sp_;
    this->cons_ = tmp.cons_;
    return *this;
}

void MainServer::TestCode(Connection &tmp, Server server)
{
    Request req;
    req.method_ = "GET";
    req.version_ = "HTTP/1.1";
    req.url_ = "/index.html";
    req.header_.insert(std::pair<std::string, std::string>("Host", "server1"));

    server.handleRequest(req, tmp);
    tmp.addFile(*this);
}

MainServer::MainServer(std::string fileName)
{
    std::ifstream inputFile(fileName.c_str());
    if (true == inputFile.is_open())
    {

        std::string s;
        std::string data;
        while (inputFile)
        {
            getline(inputFile, s);
            data.append(s);
            data.append("\r\n");
        }
        inputFile.close();
        makeServerPool(data);
    }
    else
        std::cout << "Config file open fail" << std::endl;

    std::ifstream mimeFile(std::string("mime.types").c_str());
    if (true == mimeFile.is_open())
    {

        std::string s;
        std::string data;
        while (mimeFile)
        {
            getline(mimeFile, s);
            data.append(s);
            data.append("\r\n");
        }
        mimeFile.close();
        makeMimeType(data);
    }
    else
        std::cout << "MimeType file open fail" << std::endl;
}

void MainServer::init(void)
{
    struct sockaddr_in serv_addr;
    int server_sock;
    std::string recieveString;

    std::vector<Server>::iterator it;
    std::vector<Server>::iterator its = sp_.serverPool_.begin();
    std::vector<Server>::iterator ite = sp_.serverPool_.end();

    _epfd = epoll_create(EPOLL_SIZE); // epoll 인스턴스 생성
    this->cons_.setEpfd(_epfd);
    _ep_events_buf = new epoll_event[EPOLL_SIZE]; // 버퍼 동적할당

    for (it = its; it != ite; it++)
    {
        memset(&serv_addr, 0x00, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(it->port_);

        if ((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
            std::cout << "socket create error" << std::endl;
        }

        int flags = fcntl(server_sock, F_GETFL);
        flags |= O_NONBLOCK;
        if (fcntl(server_sock, F_SETFL, flags) < 0)
        {
            std::cout << "server_fd fcntl() error" << std::endl;
            close(server_sock);
            _exit(1);
        }

        int on = 1;
        if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        {
            std::cout << "socket option set error" << std::endl;
            close(server_sock);
            _exit(1);
        }

        if (bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            std::cout << "bind error" << std::endl;
            close(server_sock);
            _exit(1);
        }

        if (listen(server_sock, 5) < 0) // 대기큐 5라고 써서 대기큐가 모지랄수도(?)
        {
            std::cout << "listen error" << std::endl;
            close(server_sock);
            _exit(1);
        }
        this->cons_.addConnection(server_sock, SERVER, "127.0.0.1");
    }
}

void MainServer::makeServerPool(std::string data)
{
    size_t endPos;
    std::string tmp;

    endPos = data.find("\r\n");
    while (endPos != std::string::npos)
    {
        tmp = data.substr(0, endPos + 2);
        data.erase(0, endPos + 2);
        endPos = data.find("\r\n");
        if (tmp.find("server") != std::string::npos && tmp.find("{") != std::string::npos)
        {
            sp_.serverPool_.push_back(makeServer(data));
            endPos = data.find("\r\n");
        }
    }
}

Server MainServer::makeServer(std::string &data)
{
    std::string tmp;
    Server server;
    size_t endPos;

    endPos = data.find("\r\n");
    while (endPos != std::string::npos)
    {
        tmp = data.substr(0, endPos + 2);
        data.erase(0, endPos + 2);
        endPos = data.find("\r\n");
        if (tmp.find("location") != std::string::npos && tmp.find("{") != std::string::npos)
        {
            server.locations_.push_back(makeLocation(data));
            endPos = data.find("\r\n");
        }
        else if (tmp.find("}") != std::string::npos)
        {
            break;
        }
        else
        {
            server.dataSetting(tmp);
        }
    }
    return server;
}

Location MainServer::makeLocation(std::string &data)
{
    std::string tmp;
    Location location;
    size_t endPos;

    endPos = data.find("\r\n");
    while (endPos != std::string::npos)
    {
        tmp = data.substr(0, endPos + 2);
        data.erase(0, endPos + 2);
        endPos = data.find("\r\n");
        if (tmp.find("}") != std::string::npos)
        {
            break;
        }
        else
        {
            location.dataSetting(tmp);
        }
    }
    return location;
}

void MainServer::start()
{
    while (1)
    {
        _event_cnt = epoll_wait(_epfd, _ep_events_buf, EPOLL_SIZE, -1);
        try
        {
            if (_event_cnt == -1)
            {
                std::cout << "wait() error!" << std::endl;
                break;
            }
            std::cout << "Recieve Events count : " << _event_cnt << std::endl;
            for (int i = 0; i < _event_cnt; i++)
            {
                Connection con = cons_.getConnection(_ep_events_buf[i].data.fd);
                if (con.kind_ == CLIENT || con.kind_ == FILE_READ)
                {
                    std::cout << "event fd =  " << _ep_events_buf[i].data.fd << std::endl;
                    std::cout << "kinds event =  " << _ep_events_buf[i].events << std::endl;
                }
                if (_ep_events_buf[i].events & EPOLLIN)
                {
                    handleReadEvent(_ep_events_buf[i].data.fd);
                }
                else if (_ep_events_buf[i].events & EPOLLOUT)
                {
                    std::cout << "EPOLLOUT SHIT" << std::endl;
                    handleWriteEvent(_ep_events_buf[i].data.fd);
                }
            }
        }
        catch (const ExceptionCode &e)
        {
        }
    }
}

void MainServer::makeMimeType(std::string data)
{
    size_t endPos;
    std::string tmp, tmp2, key, value;

    endPos = data.find("\r\n");
    tmp = data.substr(0, endPos + 2);
    data.erase(0, endPos + 2);
    endPos = data.find("\r\n");
    while (endPos != std::string::npos)
    {
        tmp = data.substr(0, endPos + 2);
        data.erase(0, endPos + 2);

        if (tmp.find("}") == std::string::npos)
        {
            tmp = ft_trim(tmp);
            value = tmp.substr(0, tmp.find_first_of(' '));
            tmp = tmp.erase(0, tmp.find_first_of(' '));
            key = ft_trim(tmp);
            if (key != "" && value != "")
            {
                while (key.find(" ") != std::string::npos)
                {
                    tmp2 = key.substr(0, key.find_first_of(' '));
                    key = key.erase(0, key.find_first_of(' ') + 1);
                    mime.insert(std::pair<std::string, std::string>(tmp2, value));
                }
                mime.insert(std::pair<std::string, std::string>(key, value));
            }
        }
        else
            break;
        endPos = data.find("\r\n");
    }
}

void MainServer::handleReadEvent(int event_fd)
{
    struct sockaddr_in clnt_addr;
    int client_sock;
    socklen_t addr_sz;
    std::string client_ip;
    char ip_tmp[16];

    Connection &con = cons_.getConnection(event_fd);
    if (con.kind_ == SERVER)
    {
        addr_sz = sizeof(clnt_addr);
        client_sock = accept(con.socket_, (struct sockaddr *)&clnt_addr, &addr_sz); // 이때 accept!!
        client_ip = std::string(inet_ntop(AF_INET, &clnt_addr.sin_addr, ip_tmp, INET_ADDRSTRLEN));
        this->cons_.addConnection(client_sock, CLIENT, client_ip);
    }
    else if (con.kind_ == CLIENT) // 클라이언트 소켓에서 온거라면 알맞게 처리
    {
        this->cons_.getConnection(con.socket_).makeRequest();
        TestCode(this->cons_.getConnection(con.socket_), sp_.serverPool_.at(0));
    }
    else if (con.kind_ == FILE_READ)
    {
        std::cout << std::endl
                  << std::endl;
        std::cout << "this is file Read" << std::endl;

        std::cout << "pipe_fd available" << std::endl;
        Connection &con2 = cons_.getPipeConnection(con.socket_);
        std::cout << "clinet : " << con2.socket_ << std::endl;
        std::cout << "pipe_fd : " << con2.pipe_fd[0] << std::endl;
        if (con2.response_.readFileData(con2.file_fd_) == false)
        {
            std::cout << "more file data !" << std::endl;
            write(con2.pipe_fd[1], "a", 1);
        }
        else
        {
            std::cout << "file data read done" << std::endl;
            std::cout << con.response_.file_data_.c_str() << std::endl;
            con.response_.state = READY;
        }
    }
}

void MainServer::handleWriteEvent(int event_fd)
{
    Connection &con = cons_.getConnection(event_fd);
    if (con.response_.state == READY)
    {
        std::cout << "write gogo" << std::endl;
        con.response_.send(1);
        con.response_.send(con.socket_);
        con.resetData();
        cons_.deleteConnection(con.pipe_fd[0]);
    }
    else
    {
        // write(event_fd, "a", 1);
    }
}