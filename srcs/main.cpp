#include <iostream>

#include <string>
#include <vector>
#include <csignal>

#include <algorithm>

#include "config/Config.hpp"
#include "server/Server.hpp"
#include "cgi/CGI.hpp"
#include "utils/utils.hpp"

bool	running = true;

void	sigquitHandler(int signum __attribute__((unused))){
	running = false;
}

int	main(int ac __attribute__((unused)), char **av __attribute__((unused)))
{		
	Server	server;

	std::signal(SIGINT, sigquitHandler);

	server.setConfigs(av);
	//server.display();
	server.run();
	return ( 0 );
}
