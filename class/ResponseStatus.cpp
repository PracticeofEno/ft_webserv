#include "ResponseStatus.hpp"

 ResponseStatus::ResponseStatus() 
 {
     setReserve();
 }
 ResponseStatus::~ResponseStatus() {}

 ResponseStatus::ResponseStatus(const ResponseStatus& tmp)
 {
     *this = tmp;
     setReserve();
 }

 ResponseStatus& ResponseStatus::operator=(const ResponseStatus& tmp)
 {
     this->code_ = tmp.code_;
     this->messasge_ = tmp.messasge_;
     return *this;
 }

void ResponseStatus::setCode(std::string code)
{   
    code_ = code;
    std::map<std::string, std::string>::iterator it = _reserve.find(code);
    if (it != _reserve.end())
        this->messasge_ = it->second;
    else
        this->messasge_ = "";
}

void ResponseStatus::setReserve()
{
    /*
    this->_reserve.insert("100", "Continue");
    this->_reserve.insert("101", "Switching Protocol");
    this->_reserve.insert("103", "Early Hints");
    this->_reserve.insert("200", "OK");
    this->_reserve.insert("201", "Created");
    this->_reserve.insert("202", "Accepted");
    this->_reserve.insert("203", "Non-Authoritative Information");
    this->_reserve.insert("204", "No Content");
    this->_reserve.insert("205", "Reset Content");
    this->_reserve.insert("206", "Partial Content");
    this->_reserve.insert("300", "Multiple Choice");
    this->_reserve.insert("301", "Moved Permanently");
    this->_reserve.insert("302", "Found");
    this->_reserve.insert("303", "See Other");
    this->_reserve.insert("304", "Not Modified");
    this->_reserve.insert("307", "Temporary Redirect");
    this->_reserve.insert("308", "Permanent Redirect");
    this->_reserve.insert("400", "Bad Request");
    this->_reserve.insert("401", "Unauthorized");
    this->_reserve.insert("403", "Forbidden");
    this->_reserve.insert("404", "Not Found");
    this->_reserve.insert("405", "Method Not Allowed");
    this->_reserve.insert("406", "Not Acceptable ");
    this->_reserve.insert("407", "Proxy Authentication Required ");
    this->_reserve.insert("408", "Request Timeout");
    this->_reserve.insert("409", "Conflict");
    this->_reserve.insert("410", "Gone ");
    this->_reserve.insert("411", "Length Required");
    this->_reserve.insert("412", "Precondition Failed ");
    this->_reserve.insert("413", "Payload Too Large");
    this->_reserve.insert("414", "URI Too Long ");
    this->_reserve.insert("415", "Unsupported Media Type ");
    this->_reserve.insert("416", "Requested Range Not Satisfiable");
    this->_reserve.insert("417", "Expectation Failed ");
    this->_reserve.insert("418", "I'm a teapot");
    this->_reserve.insert("422", "Unprocessable Entity");
    this->_reserve.insert("426", "Upgrade Required ");
    this->_reserve.insert("428", "Precondition Required ");
    this->_reserve.insert("429", "Too Many Requests ");
    this->_reserve.insert("431", "Request Header Fields Too Large");
    this->_reserve.insert("451", "Unavailable For Legal Reasons ");
    this->_reserve.insert("500", "Internal Server Error");
    this->_reserve.insert("501", "Not Implemented");
    this->_reserve.insert("502", "Bad Gateway");
    this->_reserve.insert("503", "Service Unavailable");
    this->_reserve.insert("504", "Gateway Timeout");
    this->_reserve.insert("505", "HTTP Version Not Supported");
    this->_reserve.insert("506", "Variant Also Negotiates ");
    this->_reserve.insert("507", "Insufficient Storage ");
    this->_reserve.insert("508", "Loop Detected");
    this->_reserve.insert("510", "Not Extended ");
    this->_reserve.insert("511", "Network Authentication Required ");
    */
}