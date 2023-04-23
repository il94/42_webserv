/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:08:07 by auzun             #+#    #+#             */
/*   Updated: 2023/04/23 03:57:51 by auzun            ###   ########.fr       */
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

std::vector<std::vector <std::string> > splitFileConfig(std::string path)
{
	std::vector<std::vector <std::string> > result;

	std::vector<std::string>			fileContent = fileToVector(path);
	std::vector<std::string>			element;
	std::vector<std::string>::iterator	start;
	std::vector<std::string>::iterator	end;

	for (start = std::find(fileContent.begin(), fileContent.end(), "server {"); start != fileContent.end(); start = std::find(start, fileContent.end(), "server {"))
	{
		end = std::find(start, fileContent.end(), "}");
		element.clear();
		for (++start; start != end; start++)
			element.push_back(*start);
		result.push_back(element);
		start = ++end;
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
	if (index == -1)
		return ("default");

	result = file[i - 1].substr(index + src.size() + 1, file[i - 1].find(';') - (index + src.size() + 1));
	return (result);
}

std::vector<std::string>	multipleFindInFileContent(const std::vector<std::string> &file, const std::string &src)
{
	std::vector<std::string>	result;
	int							index = -1;

	for (size_t i = 0; i < file.size(); i++)
	{
		index = file[i].find(src);
		if (index != -1)
			result.push_back(file[i].substr(index + src.size() + 1, file[i].find(';') - (index + src.size() + 1)));
	}
	return (result);
}