#include "Total.hpp"
#include "MainServer.hpp"

MainServer main_server;
std::map<std::string, std::string> mime;
int	main(int argc, char **argv)
{
	if (argc == 1)
	{
		main_server = MainServer("default.config");
	}
	else if (argc == 2)
	{
		main_server = MainServer(argv[1]);
	}
	else
	{
		return 0;
	}
	
	if (main_server.init())
		main_server.start();
	return 0;
}
