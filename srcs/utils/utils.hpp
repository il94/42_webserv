#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include <string>
#include <vector>

#include <algorithm>

# define RED "\033[31m"
# define PURPLE "\033[35m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define END "\033[0m"

size_t									rfind(const std::string& str, const std::string& substr);
std::string								to_string( size_t );
std::vector<std::string>				fileToVector( std::string );

bool	closeBrace(std::string &src, const int index);
bool	openBrace(std::string &src, const int index);

std::string					findInFileContent(const std::vector<std::string> &, const std::string &);
std::vector<std::string>	multipleFindInFileContent(const std::vector<std::string> &, const std::string &);

int		isFile(std::string path);
bool	isValidIP( const std::string & );

template <typename T>
void	displayElement(const bool &element, const std::string &name)
{
	std::cout << name + " = " << element << std::endl;
}

template <typename T>
void	displayElement(const unsigned long &element, const std::string &name)
{
	std::cout << name + " = " << element << std::endl;
}

template <typename T>
void	displayElement(const T &element, const std::string &name)
{
	if (element.empty() == true)
		std::cout << RED  << name << " EMPTY" << END << std::endl;
	else
		std::cout << name + " = " << element << std::endl;
}

template <typename T>
void	displayPair(const T &pair, const std::string &name)
{
	if (pair.second.empty() == true)
		std::cout << RED  << name << " EMPTY" << END << std::endl;
	else
		std::cout << name + " " << pair.first << " = " << pair.second << std::endl;
}

template <typename T>
void	displayVector(const T &vector, const std::string &name)
{
	if (vector.empty() == true)
		std::cout << RED  << name << " EMPTY" << END << std::endl;
	else
	{
		for (typename T::const_iterator it = vector.begin(); it != vector.end(); it++)
			std::cout << name + " = " << *it << std::endl;
	}
}

template <typename T>
void	displayMap(const T &map, const std::string &name)
{
	if (map.empty() == true)
		std::cout << RED  << name << " EMPTY" << END << std::endl;
	else
	{
		for (typename T::const_iterator it = map.begin(); it != map.end(); it++)
			std::cout << name + " " << it->first << " = " << it->second << std::endl;
	}
}

#endif
