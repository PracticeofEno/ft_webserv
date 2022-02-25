#include "Total.hpp"
#include "MainServer.hpp"

int	main(void)
{
	MainServer main_server("default.config");

	Server serv;
	std::string tmp = serv.getPayload(std::string("/root/ft_webserv/www/default_error_page.html"));

	std::cout << tmp << std::endl;

	return 0;
}
