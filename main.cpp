#include "Total.hpp"
#include "MainServer.hpp"

std::map<std::string, std::string> mime;

int	main()
{
	MainServer main_server("default.config");
	main_server.init();
	main_server.start();
	
	return 0;
}
