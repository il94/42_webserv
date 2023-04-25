/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:29:41 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/24 16:57:50 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include <string>
#include <vector>

#include <algorithm>

#include "server/Server.hpp"
#include "cgi/CGI.hpp"
#include "utils/utils.hpp"

int	main(int ac __attribute__((unused)), char **av __attribute((unused)))
{
	(void)ac;
	
	if (av[0] and av[1])
	{
		std::vector<std::vector <std::string> > _configs = extractServers(av[1]);

		std::vector< Server >	servers(_configs.size());

		std::cout << "=============================" << std::endl;
		for (size_t	i = 0; i < servers.size(); i++)
		{
			// displayVector(_configs[i], "Serveur :");
			servers[i].setConfig(_configs[i]);
			std::cout << "=============================" << std::endl;
		}
		// servers[0].run();


		
	}
	else
	{
		std::cout << "Default mode" << std::endl;
		Server	server;
		server.run();
	}

	return 0;
}
