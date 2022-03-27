#include "Server.hpp"
#include "MainServer.hpp"
#include "ResponseStatus.hpp"
#include "Util.hpp"
#include <sys/wait.h>

Server::Server()
{
}

Server::~Server()
{
}

Server::Server(const Server &server)
{
    *this = server;
}

Server &Server::operator=(const Server &server)
{
    this->server_name_ = server.server_name_;
    this->http_version_ = server.http_version_;
    this->error_page_path_ = server.error_page_path_;
    this->client_body_size_ = server.client_body_size_;
    this->locations_ = server.locations_;
    this->socket_ = server.socket_;
    this->port_ = server.port_;

    return *this;
}

void Server::dataSetting(std::string data)
{
    std::string key, value;
    data = ft_trim(data, " \t\n\r\f\v");
    key = data.substr(0, data.find_first_of(' '));
    data = data.erase(0, data.find_first_of(' '));
    value = ft_trim(data, " \t\n\r\f\v");
    if (key.compare("serverName") == 0)
        server_name_ = value;
    else if (key.compare("errorPagePath") == 0)
        error_page_path_ = value;
    else if (key.compare("port") == 0)
    {
        int c;
        std::istringstream ss(value);
        std::string token;
        while(std::getline(ss, token, ' ')) {
            std::stringstream(token) >> c;
            port_.push_back(c);
        }
    }
    else if (key.compare("clientBodySize") == 0)
        std::stringstream(value) >> client_body_size_;
    else if (key.compare("HTTP") == 0)
        http_version_ = value;
    
}

Response Server::handleRequest(Request &request, Connection& tmp)
{
    Response& response = tmp.response_;
    Location& location = findLocation(request.url_);

    if (location.methodCheck(request.method_) == NOT_ALLOW_METHOD)
        throw ExceptionCode(405, tmp);
    if (location.redirectionCheck() == ON)
        throw ExceptionCode(302);
    if (location.existFile(request.url_) == NO_EXIST_FILE)
    {
        if (request.method_ != "POST")
            throw ExceptionCode(404, tmp);
    }

    if (this->CheckCGI(tmp.reqeust_.url_, location))
    {
        this->CGIHandler(tmp.reqeust_, tmp, location);
    }
    if (request.method_ == "GET")
        response = GETHandler(request, location);
    else if (request.method_ == "POST")
        response = POSTHandler(request, location);
    else if (request.method_ == "DELETE")
        response = DELETEHandler(request, location);    
    return response;
}

Response Server::handleRequestCGI(Connection& tmp)
{
    Response& response = tmp.response_;

    if (tmp.reqeust_.method_ == "GET")
        response = GETHandlerCGI(tmp.response_);
    // else if (request.method_ == "POST")
    //     response = POSTHandlerCGI(request, location);
    // else if (request.method_ == "DELETE")
    //     response = DELETEHandlerCGI(request, location);    
    return response;
}

Location& Server::findLocation(std::string& url)
{
    std::string root = "/www";
    std::vector<Location>::iterator it;
    std::vector<Location>::iterator its = this->locations_.begin();
    std::vector<Location>::iterator ite = this->locations_.end();

    root.append(url);
    root = ft_rtrim(root, "/");
    for (it = its; it != ite; it++)
    {
        if (it->root_.compare(root) == 0)
        {
            return (*it);
        }
    }
    return (*(this->locations_.begin()));
}

Response Server::GETHandler(Request &request, Location& location)
{
    Response res;

    // 루트 디렉토리를 가르키면 index.html 추가해줌
    if (request.url_ == "/")
        request.url_.append("index.html");
    
    res.status_ = ResponseStatus(200);
    res.http_version_ = "HTTP/1.1";
    res.addHeader("Server", this->server_name_);
    res.addHeader("Date", generateTime());
    res.addHeader("Content-Type", searchMimeType(request.url_));
    res.addHeader("Last-Modified", location.getRecentTime(request.url_));
    res.addHeader("Content-Length", location.getFileSize(request.url_));
    res.addHeader("Connection", "Keep-Alive");
    res.file_path_ = location.getFilePath(request.url_);

    //디렉토리라면 content-type 텍스트로 바꿔주고 response_data에 
    //디렉토리리스트 html 만들어 넣은뒤에 content-length설정해주고 파일 이름 없앰
    if (location.isDir(request.url_) == DIRECTORY)
    {
        if (location.directory_listing_)
        {
            std::stringstream ss;
            res.header_["Content-Type"] = "text/html";
            res.response_data_ = location.getDirectoryList(request.url_);
            ss << res.response_data_.size();
            res.header_["Content-Length"] = ss.str();
            res.file_path_ = "";
        }
        else
        {
            res.file_path_ = location.getDirectoryDefaultFile();
            res.header_["Content-Type"] = "text/html";
            struct stat st;
            std::stringstream ss;
            lstat(res.file_path_.c_str(), &st);
            ss << st.st_size;
            res.header_["Content-Length"] = ss.str();
        }
        
    }
    return (res);
}

Response Server::GETHandlerCGI(Response &res)
{
    std::stringstream ss;
    int size = res.response_data_.size();
    int sub_size = 0;

    res.status_ = ResponseStatus(200);
    res.http_version_ = "HTTP/1.1";
    res.header_["Content-Type"] = "text/html";
    res.addHeader("Connection", "Keep-Alive");
    sub_size = res.response_data_.find("\r\n");
    size = size - sub_size - 4;
    ss << size;
    res.response_data_.erase(0, sub_size + 4);
    res.header_["Content-Length"] = ss.str();
    res.file_path_ = "";

    return (res);
}

Response Server::POSTHandler(Request &request, Location &location)
{
    Response res;
    res.file_path_ = location.getFilePath(request.url_);

    std::ofstream file;
    file.open(res.file_path_.c_str(), std::ios::out | std::ios::app);
    if (file.fail() == true)
        throw ExceptionCode(520);
    file << request._buffer;
    file.close();
    res.status_ = ResponseStatus(201);
    res.http_version_ = "HTTP/1.1";
    res.addHeader("Server", this->server_name_);
    res.addHeader("Date", generateTime());
    res.addHeader("Connection", "Keep-Alive");
    // res.addHeader("Content-Type", searchMimeType(request.url_));
    // res.addHeader("Last-Modified", getRecentTime(request.url_));
    // res.addHeader("Content-Length", getFileSize(request.url_));
    return (res);
}

Response Server::DELETEHandler(Request &request, Location& location)
{
    Response res;
    std::string path;

    path = location.getFilePath(request.url_);

    if (location.isDir(request.url_) == DIRECTORY)
    {
        if (rmdir(path.c_str()) == -1)
            throw std::exception();
    }
    else
    {
        if (unlink(path.c_str()) == -1)
        {
            throw std::exception();
        }
    }
    res.status_ = ResponseStatus(200);
    res.http_version_ = "HTTP/1.1";
    res.header_.insert(std::pair<std::string, std::string>("Date", generateTime()));
    return (res);
}

bool Server::CheckCGI(std::string url, Location& location)
{
    size_t index = url.find(".");
    std::string ext = url.substr(index + 1, std::string::npos);
    if (location.cgi_extension_.compare(ext) == 0)
        return true;
    return false;
}

void Server::CGIHandler(Request& request, Connection& con, Location& location)
{
    char** env;
    pid_t pid;
    int pip[2];

    char **arg = new char*[3];
	arg[0] = new char[10];
	arg[1] = new char[10];
    arg[2] = NULL;

	strncpy(arg[0], "php-cgi", 8);
	strncpy(arg[1], "index.php", 10);

    env = getCgiVariable(request, con, location);

    pipe(pip);
    con.pipe_read_ = pip[0];
    con.pipe_event_ = pip[1];
    con.kind_ = CGI;
    main_server.cons_.appConnection(con, pip[0], CGI, "CGI");

    pid = fork();
    if (pid == 0)
    {
        close(pip[0]);
        dup2(pip[1], STDOUT_FILENO);
        execvpe(arg[0], arg, env);
    }
    else
    {
        int status;
        close(pip[1]);
        waitpid(pid, &status, WNOHANG);
    }

    for (int i = 0 ; i < 20; i++)
        delete[] env[i];
    delete[] env;
    delete arg[0];
    delete arg[1];
    delete arg[2];
    delete[] arg;
}

//env 동적 할당해서 끝나고 해제해줘야함
char** Server::getCgiVariable(Request& request, Connection& tmp, Location& location)
{
    char** env;
    char* buf;
    int i = 0;
    std::map<std::string, std::string> env_tmp;
    std::stringstream ss;
    ss << tmp.port_;
    std::string tmp2;

    env_tmp.insert(std::pair<std::string, std::string>("AUTH_TYPE", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", "0"));
    env_tmp.insert(std::pair<std::string, std::string>("CONTENT_TYPE", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
    env_tmp.insert(std::pair<std::string, std::string>("PATH_INFO", "/"));
    env_tmp.insert(std::pair<std::string, std::string>("PATH_TRANSLATED", location.getFilePath(request.url_)));
    env_tmp.insert(std::pair<std::string, std::string>("QUERY_STRING", request.query_));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_ADDR", tmp.client_ip_));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_HOST", tmp.client_ip_));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_IDENT", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_USER", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("REQUEST_METHOD", request.method_));
    //env_tmp.insert(std::pair<std::string, std::string>("REQUEST_URI", getCgiUri(request, tmp.port_)));
    //env_tmp.insert(std::pair<std::string, std::string>("SCRIPT_NAME", request.url_));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_NAME", this->server_name_));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_PORT", ss.str()));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_SOFTWARE", "ft_webserv"));
    env_tmp.insert(std::pair<std::string, std::string>("REDIRECT_STATUS", "200"));
    env_tmp.insert(std::pair<std::string, std::string>("Protocol-Specific Meta-Variables", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("REDIRECT_STATUS", "200"));

    env = new char*[env_tmp.size() + 1];
    its = env_tmp.begin();
    ite = env_tmp.end();
    for (it = its; it != ite; it++)
    {
        tmp2 = it->first + "=" + it->second;
        buf = new char[tmp2.size() + 1];
        strncpy(buf, tmp2.c_str(), tmp2.size() + 1);
        env[i] = buf;
        i++;
    }
    env[i] = 0;
    return env;
}

std::string Server::getCgiUri(Request& req, int port)
{
    std::string tmp;
    std::stringstream ss;
    ss << port;
    tmp = "http://" + server_name_ + ":" + ss.str() + "/" + req.url_ + "/" + req.query_;
    tmp = replace_all(tmp, "//", "/");
    return tmp;
}