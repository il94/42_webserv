#include <fstream>
#include <iostream>

#include <vector>

#include <algorithm>

#include "Server.hpp"

template <typename T>
void	print_result(T vec)
{
	std::cout << "===========================" << std::endl;
	std::cout << std::endl;
	std::for_each(vec.begin(), vec.end(), print_test<std::string>);
	std::cout << std::endl;

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
	// std::for_each(result.begin(), result.end(), print_result<std::vector <std::string> >);
	return (result);
}


std::vector<Server>	getConfig( void );