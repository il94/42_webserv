/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:29:41 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/12 12:20:52 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include <string>
#include <vector>

#include <algorithm>

#include "Server.hpp"

// std::vector<Server>	configFile( std::string path );
std::vector<std::string>				fileToVector( std::string path );
std::vector<std::vector <std::string> > splitFileConfig(std::string path);

int	main(int ac, char **av)
{
	if (av[0] and av[1])
	{
		std::cout << "Config File mode" << std::endl;
		std::vector<std::vector <std::string> > configs = splitFileConfig(av[1]);

		std::vector< Server >	servers(configs.size());

		std::cout << "=============================" << std::endl;
		for (size_t	i = 0; i < servers.size(); i++)
		{
			servers[i].setConfig(configs[i]);
			std::cout << "=============================" << std::endl;
		}
		
		// getConfigs();
	}
	else
	{
		std::cout << "Default mode" << std::endl;
		Server	server("default");
	}


	// std::for_each(fileContent.begin(), fileContent.end(), print_test<std::string>);
	
	return 0;
}
