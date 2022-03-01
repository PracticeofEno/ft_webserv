#include "Server.hpp"

std::string &ltrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

std::string &rtrim(std::string &s, const char *t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

std::string &trim(std::string &s, const char *t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}

Server::Server()
{

}

Server::~Server()
{
    
}

Server::Server(const Server& server)
{
    *this = server;
}

Server& Server::operator=(const Server& server)
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
    data = trim(data);
    key = data.substr(0, data.find_first_of(' '));
    data = data.erase(0, data.find_first_of(' '));
    value = trim(data);
    if (key.compare("serverName") == 0)
        server_name_ = value;
    else if (key.compare("errorPagePath") == 0)
        error_page_path_ = value;
    else if (key.compare("root") == 0)
        root_ = value;
    else if (key.compare("port") == 0)
        std::stringstream(value) >> port_;
    else if (key.compare("clientBodySize") == 0)
        std::stringstream(value) >> client_body_size_;
    else if (key.compare("HTTP") == 0)
        http_version_ = value;
}


Response Server::handleRequest(Request& request)
{
    Response response;
    int index = findLocation(request.url_);
    if (index == NO)
        throw ExceptionCode(404);
    else
    {
        if (this->locations_[index].method_.compare("GET") == 0)
        {
            
        }
        else if (this->locations_[index].method_.compare("POST") == 0)
        {

        }
        else if (this->locations_[index].method_.compare("DELETE") == 0)
        {

        }
        else
            throw ExceptionCode(405);
    }
    return (response);
}

int Server::findLocation(std::string root)
{
    std::vector<Location>::iterator it;
    std::vector<Location>::iterator its = this->locations_.begin();
    std::vector<Location>::iterator ite = this->locations_.end();
    int i = 0;
    bool tf = false;

    for(it = its; it != ite; it++)
    {
        if (it->root_.compare(root) == 0)
        {
            tf = true;
            break;
        }
        i++;
    }
    if (!tf)
        return (NO);
    return (i);
}

std::string Server::generateTime()
{
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    printf("Time is: [%s]\n", buf);
    return (std::string(buf));
}

//getPayload에서 파일 전체를 getline으로 읽는데.. 
//이미지파일이나 다른거할때 문제가 생길수도(?)
std::string Server::getPayload(std::string path)
{
    std::string tmp;

	std::ifstream openFile(path.data());
	if( openFile.is_open() ){
		std::string line;
		while(getline(openFile, line)){
			tmp.append(line).append("\r\n");
		}
		openFile.close();
	}
    return tmp;
}