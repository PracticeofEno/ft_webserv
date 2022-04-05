#include "Total.hpp"
#include "MainServer.hpp"

MainServer main_server;
std::map<std::string, std::string> mime;
int	main()
{
	main_server = MainServer("default.config");
	if (main_server.init())
		main_server.start();
	return 0;
}
