/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:29:41 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/09 15:41:39 by ilandols         ###   ########.fr       */
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

	// if (av[0] and av[1])
	// 	server.setConfigs(av[1]);

	server.setConfigs(av);


	server.display();

	
	std::cout << "Server running in default mode :" << std::endl;
	
	// std::cout << "\tName : webserv"		<< std::endl;
	// std::cout << "\tIp   : 127.0.0.1"	<< std::endl;
	// std::cout << "\tPort : 8080"		<< std::endl;


	// server.run();

	
	return 0;
}
