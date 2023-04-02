/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:27 by auzun             #+#    #+#             */
/*   Updated: 2023/04/02 07:13:11 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void) {};

Response::~Response(void) {};

int	Response::readContent(void)
{
	std::ifstream	file;
	std::stringstream	buffer;
	std::string	path = _request.getURL();

	if (fileExist(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			return (-1);
		}
		
		buffer << file.rdbuf();
		_response = buffer.str();
		file.close();
		return (0);
	}
	return (-1);
}

int	Response::writeContent(std::string content)
{
	std::ofstream	file;
	std::string	path = _request.getURL();

	if (fileExist(path))
	{
		file.open(path.c_str());
		if (file.is_open() == false)
			return (-1);
		file << content;
		file.close();
		return (0);
	}
	return (-1);
}


int	Response::fileExist(std::string path)
{
	struct stat	stats;

	if (stat(path.c_str(), &stats) == 0)
		return 1;
	return 0;
}
