/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:29:41 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/27 17:02:39 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	
	if (av[0] and av[1])
	{
		std::vector<std::vector <std::string> > configs = extractServers(av[1]);

		std::cout << "=============================" << std::endl;
		for (size_t	i = 0; i < configs.size(); i++)
		{
			// displayVector(_configs[i], "Serveur :");
			( server.getConfig() ).setContent( configs[i] );
			server.setConfig( configs[i] );
			std::cout << "=============================" << std::endl;
		}
		server.run();
	}
	else
	{
		std::cout << "Server running in default mode :" << std::endl;
		std::cout << "\tName : webserv"		<< std::endl;
		std::cout << "\tIp   : 127.0.0.1"	<< std::endl;
		std::cout << "\tPort : 8080"		<< std::endl;
		server.run();
	}
	return 0;
}
