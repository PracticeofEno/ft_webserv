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

void MainServer::makeMimeType(std::string data)


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
        //makerMimeType(mime);

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

bool MainServer::openSocket(int port)
{
    struct sockaddr_in serv_addr;
    int server_sock;

    memset(&serv_addr, 0x00, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    if ((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        std::cout << "socket create error" << std::endl;
    }

    int flags = fcntl(server_sock, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(server_sock, F_SETFL, flags) < 0)
    {
        std::cout << "opensocket server_fd fcntl() error" << std::endl;
        close(server_sock);
        return false;
    }

    int on = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        std::cout << "socket option set error" << std::endl;
        close(server_sock);
        return false;
    }

    if (bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "bind error" << std::endl;
        close(server_sock);
        return false;
    }

    if (listen(server_sock, 128) < 0) // 대기큐 5라고 써서 대기큐가 모지랄수도(?)
    {
        std::cout << "listen error" << std::endl;
        close(server_sock);
        return false;
    }
    this->cons_.addConnection(server_sock, SERVER, "127.0.0.1", port);
    return true;
}

void MainServer::init(void)
{

    std::string recieveString;

    std::vector<Server>::iterator it;
    std::vector<Server>::iterator its = sp_.serverPool_.begin();
    std::vector<Server>::iterator ite = sp_.serverPool_.end();

    _epfd = epoll_create(EPOLL_SIZE); // epoll 인스턴스 생성
    this->cons_.setEpfd(_epfd);
    _ep_events_buf = new epoll_event[EPOLL_SIZE]; // 버퍼 동적할당

    for (it = its; it != ite; it++)
    {
        std::vector<int>::iterator it2;
        std::vector<int>::iterator its2 = it->port_.begin();
        std::vector<int>::iterator ite2 = it->port_.end();

        for (it2 = its2; it2 != ite2; it2++)
        {
            openSocket((*it2));
        }
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
    std::string name;

    endPos = data.find("\r\n");
    while (endPos != std::string::npos)
    {
        tmp = data.substr(0, endPos + 2);
        data.erase(0, endPos + 2);
        endPos = data.find("\r\n");
        if (tmp.find("location") != std::string::npos && tmp.find("{") != std::string::npos)
        {
            name = tmp.substr(tmp.find("/"), tmp.find("{") - tmp.find("/"));
            ft_trim(name, " \t\n\r\f\v");
            server.locations_.push_back(makeLocation(data, name));
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

Location MainServer::makeLocation(std::string &data, std::string name)
{
    std::string tmp;
    Location location;
    size_t endPos;

    endPos = data.find("\r\n");
    location.location_name_ = name;
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
            tmp = ft_trim(tmp, " \t\n\r\f\v");
            value = tmp.substr(0, tmp.find_first_of(' '));
            tmp = tmp.erase(0, tmp.find_first_of(' '));
            key = ft_trim(tmp, " \t\n\r\f\v");
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

    if (cons_.checkSocket(event_fd, ALL))
    {
        Connection &con = cons_.getConnection(event_fd);
        if (con.kind_ == SERVER)
        {
            addr_sz = sizeof(clnt_addr);
            client_sock = accept(con.socket_, (struct sockaddr *)&clnt_addr, &addr_sz); // 이때 accept!!
            client_ip = std::string(inet_ntop(AF_INET, &clnt_addr.sin_addr, ip_tmp, INET_ADDRSTRLEN));
            this->cons_.addConnection(client_sock, CLIENT, client_ip, con.port_);
        }
        else if (con.kind_ == CLIENT && con.disconnect_ == false) // 클라이언트 소켓에서 온거라면 알맞게 처리
        {
            try
            {
                if (con.reqeust_.getState() != DONE_REQUST)
                    con.makeRequest();
                if (con.reqeust_.getState() == DONE_REQUST)
                {
                    con.reqeust_.setLocationFile();
                    // con.reqeust_.printStartLine();
                    Server &server = sp_.getServer(con.reqeust_.header_["Host"], con.port_);
                    if (server.handleRequest(con.reqeust_, con) == false)
                    {
                        con.reqeust_.url_.append("/");
                        con.reqeust_.setLocationFile();
                        if (server.findLocation(con.reqeust_.location_) == false)
                            throw ExceptionCode(404);
                        if (server.handleRequest(con.reqeust_, con) == false)
                            throw ExceptionCode(404);
                    }
                }
            }
            catch (ExceptionCode &e)
            {
                ExceptionCode ex(e.code_, con);
                ex.location_ = e.location_;
                throw ex;
            }
        }
        else if (con.kind_ == CGI && con.disconnect_ == false)
        {
            Server &server = sp_.getServer(con.reqeust_.header_["Host"], con.port_);
            // CGI실행 결과를 받음.
            con.response_.readPipe(con.pipe_read_);
            std::cout << con.response_.response_data_ << std::endl;
            con.response_ = server.handleRequestCGI(con);
            con.response_.state = READY;

            epoll_event ep_event;
            ep_event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLERR | EPOLLRDHUP;
            ep_event.data.fd = con.socket_;
            epoll_ctl(_epfd, EPOLL_CTL_MOD, con.socket_, &ep_event);
            close(con.pipe_read_);
            // this->cons_.deletePipeConnection(con.pipe_read_);
        }
    }
}

void MainServer::handleWriteEvent(int event_fd)
{
    Connection &con = cons_.getConnection(event_fd);
    //연결이 끊어졌다면
    if (con.disconnect_ == true || con.disconnect_ == true)
    {
        std::cout << "disconnect true delete" << std::endl;
        this->cons_.deleteConnection(con);
    }
    else
    {
        if (con.response_.state == READY)
        {
            if (con.kind_ != CGI)
            {
                con.response_.send(con.socket_);
                con.resetData();
                this->cons_.deleteConnection(con);
            }
            else if (con.kind_ == CGI)
            {
                con.response_.send(con.socket_);
                con.resetData();
                this->cons_.deleteConnection(con);
            }
        }
    }
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
            std::cout << "client_count : " << this->cons_.cons_.size() - 2 << std::endl;
            for (int i = 0; i < _event_cnt; i++)
            {
                // std::cout << this->cons_.cons_.size() << std::endl;
                //  std::cout << "event fd : " << _ep_events_buf[i].data.fd << std::endl;
                if (_ep_events_buf[i].events & EPOLLERR || _ep_events_buf[i].events & EPOLLRDHUP)
                {
                    std::cout << "EPOLLRDHYP errror" << std::endl;
                    std::cout << "set disconnect flag true" << std::endl;
                    cons_.getConnection(_ep_events_buf[i].data.fd).disconnect_ = true;
                }
                if (_ep_events_buf[i].events & EPOLLIN)
                {
                    handleReadEvent(_ep_events_buf[i].data.fd);
                }
                if (_ep_events_buf[i].events & EPOLLOUT)
                {
                    handleWriteEvent(_ep_events_buf[i].data.fd);
                }
            }
        }
        catch (ExceptionCode &e)
        {
            e.handleException();
        }
    }
}