#include "Server.hpp"
#include "MainServer.hpp"

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
    this->port_ = server.port_;
    this->root_ = server.root_;
    this->error_page_path_ = server.error_page_path_;
    this->client_body_size_ = server.client_body_size_;
    this->server_name_ = server.server_name_;
    this->socket_ = server.socket_;
    this->locations_ = server.locations_;
    return *this;
}

void Server::dataSetting(std::string data)
{
    std::string key, value;
    data = ft_trim(data);
    key = data.substr(0, data.find_first_of(' '));
    data = data.erase(0, data.find_first_of(' '));
    value = ft_trim(data);
    if (key.compare("serverName") == 0)
        server_name_ = value;
    else if (key.compare("errorPagePath") == 0)
        error_page_path_ = value;
    else if (key.compare("root") == 0)
        root_ = value;
    else if (key.compare("cgiExtension") == 0)
        cgi_extension_ = value;
    else if (key.compare("port") == 0)
        std::stringstream(value) >> port_;
    else if (key.compare("clientBodySize") == 0)
        std::stringstream(value) >> client_body_size_;
    else if (key.compare("HTTP") == 0)
        http_version_ = value;
    
}

Response Server::handleRequest(Request &request, Connection& tmp)
{
    Response& response = tmp.response_;
    int index = findLocation(request.url_);

    if (index == NOTEXIST)
        throw ExceptionCode(404);
    else
    {
        if (index != EXIST) // location안에 존재했다면
        {
            if (this->locations_[index].methodCheck(request.method_))
            {
                response = tryHandle(request, index, tmp);
            }
            else
                throw ExceptionCode(405);
        }
        else // location에서 찾은건 아니지만 /www/를 기준으로 해당 경로에 존재한다면
        {
            if (request.method_ == "GET" || request.method_ == "POST" || request.method_ == "DELETE")
                response = tryHandle(request, tmp);
        }
    }
    return (response);
}

int Server::findLocation(std::string& url)
{
    std::vector<Location>::iterator it;
    std::vector<Location>::iterator its = this->locations_.begin();
    std::vector<Location>::iterator ite = this->locations_.end();
    int i = 0;
    bool tf = false;
    std::string path;
    char buf[PATH_MAX];
    realpath(".", buf);
    std::string current_path(buf);

    for (it = its; it != ite; it++)
    {
        if (it->root_.compare(url) == 0)
        {
            path = current_path + it->root_ + url;
            path = replace_all(path, "//", "/");
            if (access(path.c_str(), F_OK) == 0)
                tf = true;
            break;
        }
        i++;
    }
    if (!tf)
    {
        path = getFilePath(url);
        if (access(path.c_str(), F_OK) == 0)
        {
            return (EXIST);
        }
        return (NOTEXIST);
    }
    return (i);
}

std::string Server::generateTime()
{
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    return (std::string(buf));
}

Response Server::GETHandler(Request &request)
{
    Response res;
    res.status_ = ResponseStatus(200);
    res.http_version_ = "HTTP/1.1";
    res.addHeader("Server", this->server_name_);
    res.addHeader("Date", generateTime());
    res.addHeader("Content-Type", searchMimeType(request.url_));
    res.addHeader("Last-Modified", getRecentTime(request.url_));
    res.addHeader("Content-Length", getFileSize(request.url_));
    //res.addHeader("Accpet-Ranges", "bytes");
    res.file_path_ = getFilePath(request.url_);
    return (res);
}

Response Server::POSTHandler(Request &request)
{
    Response res;
    (void)request;
    return (res);
}

Response Server::DELETEHandler(Request &request)
{
    Response res;
    std::string path;

    path = getFilePath(request.url_);
    if (path.find("..") == std::string::npos)
    {
        if (unlink(path.c_str()) == -1)
        {
            throw ExceptionCode()
        }
    }
    else
        throw ExceptionCode(404);
    return (res);
}
std::string Server::searchMimeType(std::string url)
{
    std::string ret = "application/octet-stream";
    size_t index = url.find(".");
    MIMETYPE::iterator it;
    MIMETYPE::iterator its = mime.begin();
    MIMETYPE::iterator ite = mime.end();
    if (index != std::string::npos)
    {
        std::string ext = url.substr(index + 1, std::string::npos);
        if (ext.compare("") != 0)
        {
            for (it = its; it != ite; it++)
            {
                if (it->first.compare(ext) == 0)
                {
                    ret = it->second;
                    break;
                }
            }
        }
    }
    return ret;
}

std::string Server::getRecentTime(std::string url)
{
    std::string path;
    std::string ret;
    struct stat st;

    path = getFilePath(url);
    lstat(path.c_str(), &st);
    ret = std::string(ctime(&st.st_mtime));
    ret.erase(ret.find("\n"), 1);
    return (ret);
}

std::string Server::getFileSize(std::string url)
{
    std::string path;
    struct stat st;
    std::stringstream ss;

    path = getFilePath(url);
    lstat(path.c_str(), &st);
    ss << st.st_size;
    return (ss.str());
}

std::string Server::getFilePath(std::string url)
{
    std::vector<Location>::iterator it;
    std::vector<Location>::iterator its = this->locations_.begin();
    std::vector<Location>::iterator ite = this->locations_.end();
    std::string path;
    char buf[PATH_MAX];
    realpath(".", buf);
    std::string current_path(buf);

    for (it = its; it != ite; it++)
    {
        if (it->root_.compare(url) == 0)
        {
            path = current_path + it->root_ + url;
            path = replace_all(path, "//", "/");
            return (path);
        }
    }
    url = current_path + "/www/" + url;
    url = replace_all(url, "//", "/");
    return url;
}

Response Server::tryHandle(Request& req, Connection& tmp)
{
    Response res;
    

    if (CheckCGI(req.url_))
    {
        CGIHandler(req, tmp);
    }
    else
    {
        if (req.method_ == "GET")
            res = GETHandler(req);
        else if (req.method_ == "POST")
            res = POSTHandler(req);
        else if (req.method_ == "DELETE")
            res = DELETEHandler(req);
    }    
    return res;
}

Response Server::tryHandle(Request& req, int index, Connection& tmp)
{
    Response res;
    
    if (CheckCGI(req.url_, index))
    {
        CGIHandler(req, tmp);
    }
    else
    {
        if (req.method_ == "GET")
            res = GETHandler(req);
        else if (req.method_ == "POST")
            res = POSTHandler(req);
        else if (req.method_ == "DELETE")
            res = DELETEHandler(req);
    }    
    return res;
}

bool Server::CheckCGI(std::string url)
{
    size_t index = url.find(".");
    std::string ext = url.substr(index + 1, std::string::npos);
    if (this->cgi_extension_.compare(ext) == 0)
        return true;
    return false;
}

bool Server::CheckCGI(std::string url, int loca_index)
{
    size_t index = url.find(".");
    std::string ext = url.substr(index + 1, std::string::npos);

    if (this->locations_[loca_index].cgi_extension_.compare(ext) == 0)
        return true;
    return false;
}

void Server::CGIHandler(Request& request, Connection& tmp)
{
    char** env;
    pid_t pid;
    int pipe_fd[2];
    env = getCgiVariable(request, tmp);
    pipe(pipe_fd);
    main_server.cons_.addConnection(pipe_fd[0], CGI, "CGI");
    pid = fork();
    if (pid == 0)
    {
        
    }

    for (int i = 0 ; i < 20; i++)
        delete[] env[i];
    delete[] env;
}

//env 동적 할당해서 끝나고 해제해줘야함
char** Server::getCgiVariable(Request& request, Connection& tmp)
{
    char** env;
    char* buf;
    int i = 0;
    std::map<std::string, std::string> env_tmp;
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator its;
    std::map<std::string, std::string>::iterator ite;
    std::stringstream ss;
    ss << this->port_;
    std::string tmp2;

    env_tmp.insert(std::pair<std::string, std::string>("AUTH_TYPE", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", "-1"));
    env_tmp.insert(std::pair<std::string, std::string>("CONTENT_TYPE", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
    env_tmp.insert(std::pair<std::string, std::string>("PATH_INFO", request.url_));
    env_tmp.insert(std::pair<std::string, std::string>("PATH_TRANSLATED", getFilePath(request.url_)));
    env_tmp.insert(std::pair<std::string, std::string>("QUERY_STRING", request.query_));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_ADDR", tmp.client_ip_));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_HOST", tmp.client_ip_));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_IDENT", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_USER", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("REQUEST_METHOD", request.method_));
    env_tmp.insert(std::pair<std::string, std::string>("REQUEST_URI", getCgiUri(request)));
    env_tmp.insert(std::pair<std::string, std::string>("SCRIPT_NAME", request.url_));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_NAME", this->server_name_));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_PORT", ss.str()));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", this->http_version_));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_SOFTWARE", "ft_webserv"));
    env_tmp.insert(std::pair<std::string, std::string>("Protocol-Specific Meta-Variables", "null"));

    env = new char*[env_tmp.size() + 1];
    for (it = its; it != ite; it++)
    {
        tmp2 = it->first + "=" + it->second;
        buf = new char[tmp2.size() + 1];
        strncpy(buf, tmp2.c_str(), tmp2.size());
        env[i] = buf;
        i++;
    }
    return env;
}

std::string Server::getCgiUri(Request& req)
{
    std::string tmp;
    std::stringstream ss;
    ss << port_;
    tmp = "http://" + server_name_ + ":" + ss.str() + "/" + req.url_ + "/" + req.query_;
    tmp = replace_all(tmp, "//", "/");
    return tmp;
}