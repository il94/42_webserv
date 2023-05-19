#include <iostream>

#include <string>
#include <vector>

#include <algorithm>

#include "config/Config.hpp"
#include "server/Server.hpp"
#include "cgi/CGI.hpp"
#include "utils/utils.hpp"

int	main(int ac __attribute__((unused)), char **av __attribute((unused)))
{		
	Server	server;

	server.setConfigs(av);
	//server.display();
	server.run();
	return ( 0 );
}
