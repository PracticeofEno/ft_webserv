#include "Server.hpp"

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

Response Server::handleRequest(Request &request)
{
    Response response;
    int index = findLocation(request.url_);

    if (index == NOTEXIST)
        throw ExceptionCode(404);
    else
    {
        if (index != EXIST) // location안에 존재했다면
        {
            if (this->locations_[index].methodCheck(request.method_))
            {
                tryHandle(request, index);
            }
            else
                throw ExceptionCode(405);
        }
        else // location에서 찾은건 아니지만 /www/를 기준으로 해당 경로에 존재한다면
        {
            if (request.method_ == "GET" || request.method_ == "POST" || request.method_ == "DELETE")
                response = tryHandle(request);
        }
    }
    return (response);
}

int Server::findLocation(std::string url)
{
    std::vector<Location>::iterator it;
    std::vector<Location>::iterator its = this->locations_.begin();
    std::vector<Location>::iterator ite = this->locations_.end();
    int i = 0;
    bool tf = false;
    std::string path;

    for (it = its; it != ite; it++)
    {
        if (it->root_.compare(url) == 0)
        {
            path = "/root/ft_webserv/" + it->root_ + url;
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
    (void)request;
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

    for (it = its; it != ite; it++)
    {
        if (it->root_.compare(url) == 0)
        {
            path = "/root/ft_webserv/" + it->root_ + url;
            path = replace_all(path, "//", "/");
            return (path);
        }
    }
    url = "/root/ft_webserv/www/" + url;
    url = replace_all(url, "//", "/");
    return url;
}

Response Server::tryHandle(Request& req)
{
    Response res;
    

    if (CheckCGI(req.url_))
    {
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

Response Server::tryHandle(Request& req, int index)
{
    Response res;
    
    if (CheckCGI(req.url_, index))
    {
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

void Server::CGIHandler(Request& request)
{
    char** tmp;
    pid_t pid;
    
    tmp = getCgiVariable(request);
    pid = fork();
    if (pid == 0)
    {

    }
    (void)request;
}

char** Server::getCgiVariable(Request& request)
{
    char** ctmp;
    (void)request;
    std::map<std::string, std::string> tmp;

    ctmp = new char*[18];

    tmp.insert(std::pair<std::string, std::string>("AUTH_TYPE", "null"));
    tmp.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", "-1"));
    tmp.insert(std::pair<std::string, std::string>("CONTENT_TYPE", "null"));
    tmp.insert(std::pair<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
    tmp.insert(std::pair<std::string, std::string>("PATH_INFO", "?"));
    tmp.insert(std::pair<std::string, std::string>("PATH_TRANSLATED", "null"));
    tmp.insert(std::pair<std::string, std::string>("QUERY_STRING", "null"));
    tmp.insert(std::pair<std::string, std::string>("REMOTE_ADDR", "null"));
    tmp.insert(std::pair<std::string, std::string>("REMOTE_HOST", "null"));
    tmp.insert(std::pair<std::string, std::string>("REMOTE_IDENT", "null"));
    tmp.insert(std::pair<std::string, std::string>("REMOTE_USER", "null"));
    tmp.insert(std::pair<std::string, std::string>("REQUEST_METHOD", "null"));
    tmp.insert(std::pair<std::string, std::string>("SCRIPT_NAME", "null"));
    tmp.insert(std::pair<std::string, std::string>("SERVER_NAME", "null"));
    tmp.insert(std::pair<std::string, std::string>("SERVER_PORT", "null"));
    tmp.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", "null"));
    tmp.insert(std::pair<std::string, std::string>("SERVER_SOFTWARE", "null"));
    tmp.insert(std::pair<std::string, std::string>("Protocol-Specific Meta-Variables", "null"));

    return 0;
}