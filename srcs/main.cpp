/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:29:41 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/12 00:55:18 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "Server.hpp"
#include "../cgi/CGI.hpp"

// #include "cls.hpp"
// #include "cls2.hpp"

int	main()
{
	//Server server __attribute__((unused));
	// int i;
	CGI	test;
	std::cout << "1212" << std::endl << test.execCGI("test_script/bin/a.out") << std::endl;
	//server.run();
	// cls test __attribute__((unused));
	// cls2 test2 __attribute__((unused));

	return 0;
}
