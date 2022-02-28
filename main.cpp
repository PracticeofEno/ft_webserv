#include "Total.hpp"
#include "MainServer.hpp"

int	main(void)
{
	MainServer main_server("default.config");
	main_server.init();
	main_server.start();

	return 0;
}
