#include "Total.hpp"
#include "MainServer.hpp"

MainServer main_server;
std::map<std::string, std::string> mime;
std::string server;
int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Need config file" << std::endl;
		return 0;
	}
	server = argv[1];
	main_server = MainServer(server);
	if (main_server.init())
		main_server.start();
	return 0;
}
