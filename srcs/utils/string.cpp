/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:08:07 by auzun             #+#    #+#             */
/*   Updated: 2023/04/23 14:28:30 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string	to_string(size_t n)
{
	std::stringstream tmp;

	tmp << n;
	return tmp.str();
}

std::vector<std::string> fileToVector( std::string path )
{
	std::ifstream	toRead(path.c_str());

	if	(!toRead.good())
	{
		std::cout << "Invalid path file" << std::endl;
		quick_exit(EXIT_FAILURE);
	}

	std::string 				buffer;
	std::vector<std::string>	result;

	while (std::getline(toRead, buffer))
		result.push_back(buffer);
	toRead.close();
	return (result);
}

bool	closeBrace(std::string &src, const int index)
{
	std::string::iterator it = src.begin() + index - 1;
	while (it != src.end() and (*it == ' ' or *it == '\t'))
		it++;
	return (*it == '}');
}

bool	openBrace(std::string &src, const int index)
{
	std::string::iterator it = src.begin() + index - 1;
	while (it != src.end() and (*it == ' ' or *it == '\t'))
		it++;
	return (*it == '{');
}

std::vector<std::vector <std::string> > extractServers(std::string path)
{
	std::vector<std::vector <std::string> > result;

	std::vector<std::string>			fileContent = fileToVector(path);
	std::vector<std::string>::iterator	start;
	std::vector<std::string>::iterator	end;

	for (start = fileContent.begin(); start != fileContent.end(); start = end)
	{
		end = start + 1;
		if (start->rfind("server", 0, sizeof("server") - 1) != -1 and openBrace(*start, start->find("server", 0) + sizeof("server")))
		{
			while (end != fileContent.end() and (*end)[0] != '}' and end->find("server") == -1)
				end++;
			if (end != fileContent.end() and (*end)[0] == '}')
				result.push_back(std::vector<std::string>(start + 1, end));
		}
	}
	return (result);
}

std::string	findInFileContent(const std::vector<std::string> &file, const std::string &src)
{
	std::string	result;
	int			index = -1;
	size_t		i = 0;

	for (i = 0; i < file.size() and index == -1 ; i++)
		index = file[i].find(src);
	if (index != -1)
		result = file[i - 1].substr(index + src.size() + 1, file[i - 1].find(';') - (index + src.size() + 1));
	return (result);
}

std::vector<std::string>	multipleFindInFileContent(const std::vector<std::string> &file, const std::string &src)
{
	std::vector<std::string>	result;
	int							index;
	std::string::const_iterator it;

	for (std::vector<std::string>::const_iterator start = file.begin(); start != file.end(); start++)
	{
		it = start->begin();
		while (it != start->end() and (*it == ' ' or *it == '\t'))
			it++;
		index = start->rfind(src.c_str(), std::distance(start->begin(), it));
		if (index != -1 and start->find(';', index)) 
		{
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			result.push_back(start->substr(index + src.size() + 1, start->find(';') - (index + src.size() + 1)));
		}
	}
	return (result);
}
