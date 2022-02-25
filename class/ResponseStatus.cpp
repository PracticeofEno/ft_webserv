#include "ResponseStatus.hpp"

 ResponseStatus::ResponseStatus() 
 {
     setReserve();
 }

 ResponseStatus::ResponseStatus(int code)
 {
     setCode(std::to_string(code));
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
    this->_reserve.insert(std::pair<std::string, std::string>("100","Continue"));
    this->_reserve.insert(std::pair<std::string, std::string>("100", "Continue"));
    this->_reserve.insert(std::pair<std::string, std::string>("101", "Switching Protocol"));
    this->_reserve.insert(std::pair<std::string, std::string>("103", "Early Hints"));
    this->_reserve.insert(std::pair<std::string, std::string>("200", "OK"));
    this->_reserve.insert(std::pair<std::string, std::string>("201", "Created"));
    this->_reserve.insert(std::pair<std::string, std::string>("202", "Accepted"));
    this->_reserve.insert(std::pair<std::string, std::string>("203", "Non-Authoritative Information"));
    this->_reserve.insert(std::pair<std::string, std::string>("204", "No Content"));
    this->_reserve.insert(std::pair<std::string, std::string>("205", "Reset Content"));
    this->_reserve.insert(std::pair<std::string, std::string>("206", "Partial Content"));
    this->_reserve.insert(std::pair<std::string, std::string>("300", "Multiple Choice"));
    this->_reserve.insert(std::pair<std::string, std::string>("301", "Moved Permanently"));
    this->_reserve.insert(std::pair<std::string, std::string>("302", "Found"));
    this->_reserve.insert(std::pair<std::string, std::string>("303", "See Other"));
    this->_reserve.insert(std::pair<std::string, std::string>("304", "Not Modified"));
    this->_reserve.insert(std::pair<std::string, std::string>("307", "Temporary Redirect"));
    this->_reserve.insert(std::pair<std::string, std::string>("308", "Permanent Redirect"));
    this->_reserve.insert(std::pair<std::string, std::string>("400", "Bad Request"));
    this->_reserve.insert(std::pair<std::string, std::string>("401", "Unauthorized"));
    this->_reserve.insert(std::pair<std::string, std::string>("403", "Forbidden"));
    this->_reserve.insert(std::pair<std::string, std::string>("404", "Not Found"));
    this->_reserve.insert(std::pair<std::string, std::string>("405", "Method Not Allowed"));
    this->_reserve.insert(std::pair<std::string, std::string>("406", "Not Acceptable "));
    this->_reserve.insert(std::pair<std::string, std::string>("407", "Proxy Authentication Required "));
    this->_reserve.insert(std::pair<std::string, std::string>("408", "Request Timeout"));
    this->_reserve.insert(std::pair<std::string, std::string>("409", "Conflict"));
    this->_reserve.insert(std::pair<std::string, std::string>("410", "Gone "));
    this->_reserve.insert(std::pair<std::string, std::string>("411", "Length Required"));
    this->_reserve.insert(std::pair<std::string, std::string>("412", "Precondition Failed "));
    this->_reserve.insert(std::pair<std::string, std::string>("413", "Payload Too Large"));
    this->_reserve.insert(std::pair<std::string, std::string>("414", "URI Too Long "));
    this->_reserve.insert(std::pair<std::string, std::string>("415", "Unsupported Media Type "));
    this->_reserve.insert(std::pair<std::string, std::string>("416", "Requested Range Not Satisfiable"));
    this->_reserve.insert(std::pair<std::string, std::string>("417", "Expectation Failed "));
    this->_reserve.insert(std::pair<std::string, std::string>("418", "I'm a teapot"));
    this->_reserve.insert(std::pair<std::string, std::string>("422", "Unprocessable Entity"));
    this->_reserve.insert(std::pair<std::string, std::string>("426", "Upgrade Required "));
    this->_reserve.insert(std::pair<std::string, std::string>("428", "Precondition Required "));
    this->_reserve.insert(std::pair<std::string, std::string>("429", "Too Many Requests "));
    this->_reserve.insert(std::pair<std::string, std::string>("431", "Request Header Fields Too Large"));
    this->_reserve.insert(std::pair<std::string, std::string>("451", "Unavailable For Legal Reasons "));
    this->_reserve.insert(std::pair<std::string, std::string>("500", "Internal Server Error"));
    this->_reserve.insert(std::pair<std::string, std::string>("501", "Not Implemented"));
    this->_reserve.insert(std::pair<std::string, std::string>("502", "Bad Gateway"));
    this->_reserve.insert(std::pair<std::string, std::string>("503", "Service Unavailable"));
    this->_reserve.insert(std::pair<std::string, std::string>("504", "Gateway Timeout"));
    this->_reserve.insert(std::pair<std::string, std::string>("505", "HTTP Version Not Supported"));
    this->_reserve.insert(std::pair<std::string, std::string>("506", "Variant Also Negotiates "));
    this->_reserve.insert(std::pair<std::string, std::string>("507", "Insufficient Storage "));
    this->_reserve.insert(std::pair<std::string, std::string>("508", "Loop Detected"));
    this->_reserve.insert(std::pair<std::string, std::string>("510", "Not Extended "));
    this->_reserve.insert(std::pair<std::string, std::string>("511", "Network Authentication Required "));
}