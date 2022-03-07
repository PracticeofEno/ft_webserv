#include "MainServer.hpp"

void TestCode(Connection& tmp, Server server)
{
    Request req;
    req.method_ = "GET";
    req.version_ = "HTTP/1.1";
    req.url_ = "/default_error_page.html";
    req.header_.insert(std::pair<std::string, std::string>("Host", "server1"));
    
    Response res = server.handleRequest(req);
    res.send(1);
    res.send(tmp.socket_);
}

std::string &lltrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

std::string &rrtrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

std::string &ttrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    return lltrim(rrtrim(s, t), t);
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

MainServer::~MainServer()
{
}

void MainServer::init(void)
{
    struct sockaddr_in serv_addr;
    int server_sock;
    std::string recieveString;

    std::vector<Server>::iterator it;
    std::vector<Server>::iterator its = sp_.serverPool_.begin();
    std::vector<Server>::iterator ite = sp_.serverPool_.end();

    _epfd = epoll_create(EPOLL_SIZE);             // epoll 인스턴스 생성
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
        this->cons_.addConnection(server_sock, SERVER);
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
    struct sockaddr_in clnt_addr;
    int client_sock;
    socklen_t addr_sz;

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
            std::cout << _event_cnt << std::endl;
            for (int i = 0; i < _event_cnt; i++)
            {
                // ConnectionPool 뒤적거려서 해당 소켓이 서버소켓인지 확인한뒤에 서버라면
                if (this->cons_.CheckSocket(_ep_events_buf[i].data.fd, SERVER))
                {
                    addr_sz = sizeof(clnt_addr);
                    client_sock = accept(_ep_events_buf[i].data.fd, (struct sockaddr *)&clnt_addr, &addr_sz); // 이때 accept!!
                    this->cons_.addConnection(client_sock, CLIENT);
                }
                else // 클라이언트 소켓에서 온거라면 알맞게 처리
                {
                    // Request 처리
                    if (this->cons_.CheckSocket(_ep_events_buf[i].data.fd, CLIENT))
                    {
                        this->cons_.getConnection(_ep_events_buf[i].data.fd).makeRequest();
                        TestCode(this->cons_.getConnection(_ep_events_buf[i].data.fd), sp_.serverPool_.at(0));
                    }
                    else
                    {
                        //사실 여기에 흐름이 오면 안되겠지만..
                        // Coonection에 없는애가 요청이 온상태임
                    }
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
            tmp = ttrim(tmp);
            value = tmp.substr(0, tmp.find_first_of(' '));
            tmp = tmp.erase(0, tmp.find_first_of(' '));
            key = ttrim(tmp);
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