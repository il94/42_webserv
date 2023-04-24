/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:08:07 by auzun             #+#    #+#             */
/*   Updated: 2023/04/24 18:09:37 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

#include "utils.hpp"

size_t rfind(const std::string& str, const std::string& substr)
{
	size_t pos = str.size();
	size_t sub_len = substr.size();

	if (sub_len > pos)
		return std::string::npos;
	for (size_t i = pos - sub_len; i != std::string::npos; --i)
	{
		if (str.compare(i, sub_len, substr) == 0)
			return i;
	}
	return std::string::npos;
}

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
		if (start->rfind("server", 0, sizeof("server") - 1) != std::string::npos and openBrace(*start, start->find("server", 0) + sizeof("server")))
		{
			while (end != fileContent.end() and (*end)[0] != '}')
				end++;
			if (end != fileContent.end() and (*end)[0] == '}')
				result.push_back(std::vector<std::string>(start + 1, end));
		}
	}
	return (result);
}

std::string	findInFileContent(const std::vector<std::string> &file, const std::string &src)
{
	std::string					result;
	int							index;
	std::string::const_iterator it;

	for (std::vector<std::string>::const_iterator start = file.begin(); start != file.end(); start++)
	{
		it = start->begin();
		while (it != start->end() and (*it == ' ' or *it == '\t'))
			it++;
		index = start->rfind(src.c_str(), std::distance(start->begin(), it));
		if (index != -1 and (*(it + src.size()) == ' ' or *(it + src.size()) == '\t') and start->find(';', index) != std::string::npos) 
		{
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			while (it != start->end() and *it != ' ' and *it != '\t')
				it++;
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			if (it != start->end())
				result = start->substr(std::distance(start->begin(), it), start->find(';') - std::distance(start->begin(), it));
			return (result);
		}
	}
	return (result);
}

std::vector<std::string>	multipleFindInFileContent(const std::vector<std::string> &file, const std::string &src)
{
	std::vector<std::string>	result;
	size_t						index;
	std::string::const_iterator it;

	for (std::vector<std::string>::const_iterator start = file.begin(); start != file.end(); start++)
	{
		it = start->begin();
		while (it != start->end() and (*it == ' ' or *it == '\t'))
			it++;
		index = start->rfind(src.c_str(), std::distance(start->begin(), it));
		if (index != std::string::npos and (*(it + src.size()) == ' ' or *(it + src.size()) == '\t') and start->find(';', index) != std::string::npos) 
		{
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			while (it != start->end() and *it != ' ' and *it != '\t')
				it++;
			while (it != start->end() and (*it == ' ' or *it == '\t'))
				it++;
			if (it != start->end())
				result.push_back(start->substr(std::distance(start->begin(), it), start->find(';') - std::distance(start->begin(), it)));
		}
	}
	return (result);
}
