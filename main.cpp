#include "Total.hpp"
#include "MainServer.hpp"

std::map<std::string, std::string> mime;
MainServer main_server;

int	main()
{
	main_server = MainServer("default.config");
	main_server.init();
	main_server.start();
	
	return 0;
}
