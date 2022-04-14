#include "Server.hpp"
#include "MainServer.hpp"
#include "ResponseStatus.hpp"
#include "Util.hpp"
#include <sys/wait.h>
#include "MultiPart.hpp"

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
    this->client_body_size_ = server.client_body_size_;
    this->locations_ = server.locations_;
    this->socket_ = server.socket_;
    this->port_ = server.port_;
    this->error_page_ = server.error_page_;

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
    else if (key.compare("error_page") == 0)
        error_page_ = value;
    else if (key.compare("clientBodySize") == 0)
        std::stringstream(value) >> client_body_size_;
    else if (key.compare("HTTP") == 0)
        http_version_ = value;
    
}

bool Server::handleRequest(Request &request, Connection& con)
{
    Location& location = findLocation(request);

    if (request.body_.size() > (size_t)this->client_body_size_)
        throw ExceptionCode(413);
    if (location.methodCheck(request.method_) == NOT_ALLOW_METHOD)
        throw ExceptionCode(405);
    if (location.redirectionCheck() == ON)
    {
        ExceptionCode ex(302);
        ex.location_ = "http://" + location.redirection_ + location.location_name_ + request.file_;
        throw ex;
    }
    if (location.existFile(request) == NOTEXIST && request.method_ != "POST")
        return false;

    if (this->CheckCGI(request.url_, location))
        this->CGIHandler(request, con, location);
    else
    {
        if (request.method_ == "GET")
            con.response_ = GETHandler(request, location);
        else if (request.method_ == "POST")
            con.response_ = POSTHandler(request, location);
        else if (request.method_ == "DELETE")
            con.response_ = DELETEHandler(request, location);    
        con.response_.state = READY;
    }
    return true;
}

Response Server::handleRequestCGI(Connection& tmp)
{
    Response& res = tmp.response_;
    std::map<std::string, std::string> cgi_header;
    std::string str_tmp;

    while (true)
    {
        str_tmp = res.response_data_.substr(0, res.response_data_.find("\r\n") + 2);
        res.response_data_ = res.response_data_.erase(0, res.response_data_.find("\r\n") + 2);
        if (str_tmp == "\r\n" || res.response_data_ == "")
            break;
        size_t colon_index = str_tmp.find(":");
        cgi_header[str_tmp.substr(0, colon_index)] = str_tmp.substr(colon_index + 2, str_tmp.find("\r\n") - colon_index - 2);
    }

    std::stringstream ss, ss2;
    int code = 0;
    int size = 0;

    if (cgi_header.find("Status") != cgi_header.end())
    {
        ss << cgi_header["Status"];
        ss >> code;
    }
    if (cgi_header.find("Content-Type") != cgi_header.end())
    {
        res.header_["Content-Type"] = cgi_header.find("Content-Type")->second;
    }
    if (cgi_header.find("Location") != cgi_header.end())
    {
        res.header_["Location"] = cgi_header.find("Location")->second;
    }
    res.status_ = ResponseStatus(code);
    res.http_version_ = "HTTP/1.1";
    res.header_["Connection"] = "Keep-Alive";
    res.header_["Keep-Alive"] = "20";
    
    size = res.response_data_.size();
    ss2 << size;
    
    res.header_["Content-Length"] = ss2.str();
    res.file_path_ = "";   
    return res;
}

Location& Server::findLocation(Request& request)
{
    std::string url;
    int match_count = 0;
    int max_count = 0;

    std::vector<Location>::iterator it;
    std::vector<Location>::iterator its = this->locations_.begin();
    std::vector<Location>::iterator ite = this->locations_.end();
    std::vector<Location>::iterator tmp = this->locations_.begin();

    for (it = its; it != ite; it++)
    {
        if (ft_strcmp(it->location_name_.c_str(), request.location_.c_str()) == true)
        {
            match_count = it->location_name_.size();
            if (match_count > max_count)
            {
                max_count = match_count;
                tmp = it;
            }
        }
    }
    return (*tmp);
}

bool Server::findLocation(std::string location)
{
    std::string url;

    std::vector<Location>::iterator it;
    std::vector<Location>::iterator its = this->locations_.begin();
    std::vector<Location>::iterator ite = this->locations_.end();

    for (it = its; it != ite; it++)
    {
        if (it->location_name_.compare(location) == 0)
            return true;
    }
    return false;
}

Response Server::GETHandler(Request &request, Location& location)
{
    Response res;

    res.status_ = ResponseStatus(200);
    res.http_version_ = "HTTP/1.1";
    res.addHeader("Server", this->server_name_);
    res.addHeader("Date", generateTime());
    res.addHeader("Content-Type", searchMimeType(request.file_));
    res.addHeader("Last-Modified", location.getRecentTime(request.file_));
    res.addHeader("Content-Length", location.getFileSize(request.file_));
    res.addHeader("Connection", "Keep-Alive");
    res.header_["Keep-Alive"] = "20";
    res.file_path_ = location.getFilePath(request.file_);

    //디렉토리라면 content-type 텍스트로 바꿔주고 response_data에 
    //디렉토리리스트 html 만들어 넣은뒤에 content-length설정해주고 파일 이름 없앰
    if (location.isDir(request.file_) == DIRECTORY)
    {
        if (location.directory_listing_)
        {
            std::stringstream ss;
            res.header_["Content-Type"] = "text/html";
            res.response_data_ = location.getDirectoryList(request.file_, request.location_);
            ss << res.response_data_.size();
            res.header_["Content-Length"] = ss.str();
            res.file_path_ = "";
        }
        else
        {
            res.file_path_ = location.getServerRootPath(location.dl_default_);
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
    

    return (res);
}

Response Server::POSTHandler(Request &request, Location &location)
{
    Response response;

    int post_type = request.checkPostType();

    if (post_type == NONE)
    {
        ExceptionCode ex(400);
        ex.error_str = "Bad Request - Content-Type missing";
        throw ex;
    }
    else if (post_type == STATIC_POST)
    {
        int fd;

        response.file_path_ = location.getUploadPath(request.url_);// 경로찾고
        if ((fd = open(response.file_path_.c_str(), O_CREAT | O_WRONLY | O_NONBLOCK)) < 0)
            std::cout << "open error occur: " << errno << std::endl;
        if (write(fd, request.body_.c_str(), request.body_.size()) < 0)
            std::cout << "write error occur" << std::endl;

        response.status_ = ResponseStatus(201);
        response.http_version_ = "HTTP/1.1";
        response.addHeader("Server", this->server_name_);
        response.addHeader("Date", generateTime());
        response.header_["Connection"] = "Keep-alive";
        response.header_["Keep-Alive"] = "20";
        response.header_["Content-Type"] = "text/plain";
        response.response_data_ = "201 Created";
        response.header_["Content-Length"] = "11";
        response.file_path_ = "";
    }
    else if (post_type == UPLOAD_POST)
    {
        std::string boundary;
        boundary = request.header_["Content-Type"].substr(request.header_["Content-Type"].find("boundary="), std::string::npos);
        boundary.erase(0, sizeof("boundary="));
    
        MultiPart multi(request.body_, boundary);
        multi.makeEntity();
        multi.parseEntity();
        multi.writeEntity(location);

        response.status_ = ResponseStatus(201);
        response.http_version_ = "HTTP/1.1";
        response.response_data_ = "201 Created";
        response.addHeader("Content-Length", "11");
        response.addHeader("Server", this->server_name_);
        response.addHeader("Date", generateTime());
        response.header_["Connection"] = "Keep-alive";
        response.header_["Keep-Alive"] = "20";
        response.header_["Content-Type"] = "text/plain";
        response.file_path_ = "";
    }
    return (response);
}

Response Server::DELETEHandler(Request &request, Location& location)
{
    Response res;
    std::string path;

    path = location.getServerRootPath(request.url_);

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
    int pip_to_child[2];
    int pip_to_parrent[2];

    char **arg = 0;
	
    std::string tmp = location.getCgiCommand();
    if (tmp.find("ubuntu") != std::string::npos)
    {
        std::cout << location.getFilePath(request.file_) << std::endl;
        std::ifstream is(location.getFilePath(request.file_).c_str());
        if (is)
        {
            int length;
            char *buffer;
            // seekg를 이용한 파일 크기 추출
            is.seekg(0, is.end);
            length = (int)is.tellg();
            is.seekg(0, is.beg);

            // malloc으로 메모리 할당
            buffer = new char[length];
            // read data as a block:
            is.read((char *)buffer, length);
            is.close();
            request.body_.clear();
            request.body_.append(buffer);
            if (request.body_.find("\n") != std::string::npos)
                request.body_.erase(request.body_.find("\n"), std::string::npos);
            delete[] buffer;
        }
    }
    else if (tmp == "CGI-NONE")
    {
        throw ExceptionCode(999);
    }

    env = getCgiVariable(request, con, location);

    pipe(pip_to_child);
    pipe(pip_to_parrent);
    con.pipe_read_ = pip_to_parrent[0];

    int flags = fcntl(pip_to_child[1], F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(pip_to_child[1], F_SETFL, flags) < 0)
    {
        std::cout << "add connection fcntl() error" << std::endl;
        close(pip_to_child[1]);
        _exit(1);
    }

    con.kind_ = CGI;
    main_server.cons_.appConnection(pip_to_parrent[0], CGI);

    pid = fork();
    if (pid == 0)
    {
        dup2(pip_to_child[0], STDIN_FILENO);
        dup2(pip_to_parrent[1], STDOUT_FILENO);
        close(pip_to_child[1]);
        close(pip_to_parrent[0]);
        execvpe(tmp.c_str() , arg, env);
    }
    else
    {
        int status;
        close(pip_to_child[0]);
        close(pip_to_parrent[1]);
        write(pip_to_child[1], request.body_.c_str(), request.body_.size());
        close(pip_to_child[1]);
        waitpid(pid, &status, WNOHANG);
    }
    int i;
    for (i = 0 ; env[i] != 0; i++)
        delete[] env[i];
    delete[] env[i];
    delete[] env;
}

//env 동적 할당해서 끝나고 해제해줘야함
char** Server::getCgiVariable(Request& request, Connection& tmp, Location& location)
{
    char** env;
    char* buf;
    int i = 0;
    std::map<std::string, std::string> env_tmp;
    std::stringstream ss;
    std::string tmp2;

    env_tmp.insert(std::pair<std::string, std::string>("AUTH_TYPE", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("CONTENT_TYPE", "application/x-www-form-urlencoded"));
    env_tmp.insert(std::pair<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
    env_tmp.insert(std::pair<std::string, std::string>("PATH_INFO", "/"));
    env_tmp.insert(std::pair<std::string, std::string>("PATH_TRANSLATED", location.getServerRootPath(request.file_) ));
    env_tmp.insert(std::pair<std::string, std::string>("REQUEST_METHOD", request.method_));
    if (request.method_ == "GET")
    {
        env_tmp.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", "0"));
        env_tmp.insert(std::pair<std::string, std::string>("QUERY_STRING", request.query_));
    }
    else if (request.method_ == "POST")
    {
        ss << request.body_.size();
        env_tmp.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", ss.str()));
        std::cout << ss.str() << std::endl;
        env_tmp.insert(std::pair<std::string, std::string>("QUERY_STRING", request.body_));
    }

    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_ADDR", tmp.client_ip_));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_HOST", tmp.client_ip_));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_IDENT", "null"));
    env_tmp.insert(std::pair<std::string, std::string>("REMOTE_USER", "null"));
    //env_tmp.insert(std::pair<std::string, std::string>("REQUEST_URI", getCgiUri(request, tmp.port_)));
    //env_tmp.insert(std::pair<std::string, std::string>("SCRIPT_NAME", request.url_));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_NAME", this->server_name_));
    ss.str("");
    ss << tmp.port_;
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_PORT", ss.str()));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
    env_tmp.insert(std::pair<std::string, std::string>("SERVER_SOFTWARE", "ft_webserv"));
    env_tmp.insert(std::pair<std::string, std::string>("REDIRECT_STATUS", "200"));
    env_tmp.insert(std::pair<std::string, std::string>("Protocol-Specific Meta-Variables", "null"));

    env = new char*[env_tmp.size() + 1];
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator its;
    std::map<std::string, std::string>::iterator ite;
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
