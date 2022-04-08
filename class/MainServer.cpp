#include "MainServer.hpp"
#include "algorithm"
#include "Util.hpp"
#include "ExceptionCode.hpp"

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

bool MainServer::init(void)
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
            // if (openSocket((*it2)) == false)
            //     return false;
            openSocket((*it2));
        }
    }
    if (checkValidConfig() == false)
    {
        // std::cout << "Error - Wrong cofigfile" << std::endl;
        return false;
    }
    return true;
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

    Connection &con = cons_.getConnection(event_fd);
    if (con.kind_ == SERVER)
    {
        addr_sz = sizeof(clnt_addr);
        client_sock = accept(con.socket_, (struct sockaddr *)&clnt_addr, &addr_sz); // 이때 accept!!
        client_ip = std::string(inet_ntop(AF_INET, &clnt_addr.sin_addr, ip_tmp, INET_ADDRSTRLEN));
        this->cons_.addConnection(client_sock, CLIENT, client_ip, con.port_);
    }
    else if (con.kind_ == CLIENT) // 클라이언트 소켓에서 온거라면 알맞게 처리
    {
        try
        {
            if (con.reqeust_.getState() != DONE_REQUST)
                con.makeRequest();
            if (con.reqeust_.getState() == DONE_REQUST)
            {
                con.reqeust_.setLocationFile();
                con.reqeust_.printStartLine();
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
            ExceptionCode ex(e.code_);
            ex.location_ = e.location_;
            throw ex;
        }
    }
    else if (con.kind_ == CGI)
    {
        Server &server = sp_.getServer(con.reqeust_.header_["Host"], con.port_);
        // CGI실행 결과를 받음.
        con.response_.readPipe(con.pipe_read_);
        //std::cout << con.response_.response_data_ << std::endl;
        con.response_ = server.handleRequestCGI(con);
        con.response_.state = READY;
        close(con.pipe_read_);
        // this->cons_.deletePipeConnection(con.pipe_read_);
    }
}

void MainServer::handleWriteEvent(int event_fd)
{
    Connection &con = cons_.getConnection(event_fd);

    if (con.e_.code_ == 0)
    {
        if (con.response_.state == READY)
        {
            if (con.kind_ != CGI)
            {
                con.response_.send(con.socket_);
                if (con.reqeust_.header_["Connection"] == "Close" || con.reqeust_.header_["Connection"] == "close")
                    this->cons_.deleteConnection(con);
                else
                    con.resetData();
            }
            else if (con.kind_ == CGI)
            {
                con.response_.send(con.socket_);
                if (con.reqeust_.header_["Connection"] == "Close" || con.reqeust_.header_["Connection"] == "close")
                    this->cons_.deleteConnection(con);
                else
                    con.resetData();
            }
        }
    }
    else
    {
        con.handleException();
    }
}

int MainServer::eventWait()
{
    long last_time = 0;
    long current_time = 0;
    int event_cnt;

    if (current_time != 0)
        last_time = current_time;
    else
        last_time = get_time();
    if (this->cons_.cons_.size() == this->sp_.serverPool_.size())
        event_cnt = epoll_wait(_epfd, _ep_events_buf, EPOLL_SIZE, -1);
    else
    {
        // sort()는 사실 없어도 되는데.. 클라이언트가 많아지면 정렬한뒤에 첫번째놈 가져오면 되는데..
        //현재는 서버마저 CLIENT POLL에 넣어버려서 ㅋ -_- 빠르게 가져오자면 가져올순 있지만..무의미한 sort라 주석
        //바꾸기가 너무 귀찮아
        // this->cons_.sort();
        event_cnt = epoll_wait(_epfd, _ep_events_buf, EPOLL_SIZE, this->cons_.getMinTimeOut());
    }
    current_time = get_time();
    std::cout << std::endl;
    this->cons_.printPool();
    this->cons_.eraseTimeOut(current_time - last_time);
    std::cout << "=================================" << std::endl;
    this->cons_.printPool();
    std::cout << std::endl;
    return event_cnt;
}

void MainServer::start()
{
    int event_cnt;
    while (1)
    {

        event_cnt = eventWait();
        if (event_cnt == -1)
            continue;
        std::cout << "client_count : " << this->cons_.cons_.size() - 2 << std::endl;
        for (int i = 0; i < event_cnt; i++)
        {
            if (cons_.checkSocket(_ep_events_buf[i].data.fd, CLIENT))
                cons_.getConnection(_ep_events_buf[i].data.fd).timeout_ = 20000;
            if (_ep_events_buf[i].events & EPOLLERR || _ep_events_buf[i].events & EPOLLRDHUP)
            {
                cons_.getConnection(_ep_events_buf[i].data.fd).disconnect_ = true;
            }
            else if (_ep_events_buf[i].events & EPOLLIN)
            {
                Connection &tmp = cons_.getConnection(_ep_events_buf[i].data.fd);
                try
                {
                    handleReadEvent(_ep_events_buf[i].data.fd);
                }
                catch (ExceptionCode &e)
                {
                    tmp.e_ = e;
                }
                if (tmp.response_.state == READY || tmp.e_.code_ != 0)
                {
                    epoll_event ep_event;
                    ep_event.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLERR | EPOLLRDHUP;
                    ep_event.data.fd = tmp.socket_;
                    epoll_ctl(_epfd, EPOLL_CTL_MOD, tmp.socket_, &ep_event);
                }
            }
            else if (_ep_events_buf[i].events & EPOLLOUT)
            {
                handleWriteEvent(_ep_events_buf[i].data.fd);
            }
        }
    }
}

bool MainServer::checkValidConfig()
{
    std::vector<Server>::iterator it;
    std::vector<Server>::iterator its = this->sp_.serverPool_.begin();
    std::vector<Server>::iterator ite = this->sp_.serverPool_.end();

    std::vector<Location>::iterator lo_it;
    std::vector<Location>::iterator lo_its;
    std::vector<Location>::iterator lo_ite;

    for (it = its; it != ite; it++)
    {
        std::string error_page = it->locations_[0].getServerRootPath(it->error_page_);
        struct stat sb;
        if (stat(error_page.c_str(), &sb) == -1)
        {
            std::cout << "Wrong error file : " << error_page << std::endl;
            return false;
        }
        if (sb.st_mode & S_IFDIR)
        {
            std::cout << "Wrong error file - Plase check file type : " << error_page << std::endl;
            return false;
        }
        lo_its = it->locations_.begin();
        lo_ite = it->locations_.end();
        for (lo_it = lo_its; lo_it != lo_ite; lo_it++)
        {
            if (lo_it->checkValid() == false)
                return false;
        }
    }
    return true;
}