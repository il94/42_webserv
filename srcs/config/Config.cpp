#include "Config.hpp"

#include "../utils/utils.hpp"

/*=============================== Constructors ===============================*/

Config::Config(){
}

Config::Config(const Config &src){
	*this = src;
}

Config::~Config(){
}

/*================================ Overloads =================================*/

Config& Config::operator=(const Config &src)
{
	_content = src._content;
	_error = src._error;
	_port = src._port;
	_host = src._host;
	_name = src._name;
	_errorPages = src._errorPages;
	_maxBodySize = src._maxBodySize;
	_locations = src._locations;
	return (*this);
}

/*================================= Methods ==================================*/

void	Config::display( void )
{
	displayVector(getPort(), "PORT");
	displayVector(getHost(), "HOST");
	displayElement(getName(), "NAME");
	displayMap(getErrorPages(), "ERROR PAGE");
	displayElement(getMaxBodySize(), "MAX BODY SIZE");

	std::cout << std::endl;
	for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		std::cout << "LOCATION [" << it->first << "]" << std::endl;
		it->second.display();
		std::cout << std::endl;
	}
}

bool	closeBrace(std::string src)
{
	std::string::iterator it = src.begin();

	while (it != src.end() and (*it == ' ' or *it == '\t'))
		it++;
	return (*it == '}');
}

std::map<std::string, Location>	Config::extractLocations( void )
{
	std::map<std::string, Location>		result;

	std::vector<std::string>::iterator	start;
	std::vector<std::string>::iterator	end;

	std::string::iterator				it;

	for (start = _content.begin(); start != _content.end(); start++)
	{
		it = start->begin();
		while (*it == ' ' or *it == '\t')
			it++;
		if (start->substr(std::distance(start->begin(), it), sizeof("location")) == "location ")
		{
			it += sizeof("location");
			while (*it == ' ' or *it == '\t')
				it++;
			if (it != start->end())
			{
				std::pair<std::string, Location>	element;
				element.first = start->substr(std::distance(start->begin(), it), start->find(' ', std::distance(start->begin(), it + 1)) - std::distance(start->begin(), it));
				it += element.first.size();
				if (start->find("{\n", start->find(' ', std::distance(start->begin(), it + 1) - std::distance(start->begin(), it))))
				{
					start++;
					while (not closeBrace(*start))
					{
						element.second.pushContent(*start);
						start++;
					}
					if (not element.second.emptyContent())
						result.insert(std::pair<std::string, Location>(element));
				}
			}
		}
	}
	return (result);
}

std::vector<int>	Config::extractPort( void ) //verif si inferieur a 1024
{
	std::vector<int>			result;
	std::vector<std::string>	content;

	content = multipleFindInFileContent(_content, "listen");
	if (content.empty() == true)
		result.push_back(8000);
	else
	{
		for (std::vector<std::string>::iterator it = content.begin(); it < content.end(); it++)
			result.push_back(std::atoi((it->substr(it->find(':') + 1, it->size()).c_str())));
	}
	return (result);
}

std::vector<std::string>	Config::extractHost( void )
{
	std::vector<std::string>	result;
	std::vector<std::string>	content;

	content = multipleFindInFileContent(_content, "listen");
	if (content.empty() == true)
		result.push_back("127.0.0.1");
	else
	{
		for (std::vector<std::string>::iterator it = content.begin(); it < content.end(); it++)
			result.push_back(it->substr(0, it->find(':')));
	}
	return (result);
}

std::string	Config::extractName( void )
{
	std::string	result;

	result = findInFileContent(_content, "server_name");
	if (result == "default")
		return ("default_server");
	else
		return (result);
}

std::map<std::string, std::string>	Config::extractErrorPages( void )
{
	std::map<std::string, std::string>	result;
	std::vector<std::string>			content;

	content = multipleFindInFileContent(_content, "error_page");

	result.insert(std::pair<std::string, std::string>("404", "/default_files/404.html"));
	result.insert(std::pair<std::string, std::string>("500", "/default_files/500.html"));
	result.insert(std::pair<std::string, std::string>("501", "/default_files/501.html"));
	result.insert(std::pair<std::string, std::string>("502", "/default_files/502.html"));
	std::pair<std::string, std::string>	element;
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); *it++)
	{
		if (result.find(it->substr(0, it->find('/') - 1)) == result.end())
		{
			element.first = it->substr(0, it->find('/') - 1);
			element.second = it->substr(it->find('/'), ';');
			result.insert(element);
		}
		else
			result[it->substr(0, it->find('/') - 1)] = it->substr(it->find('/'), ';');
	}
	return (result);
}

long	Config::extractMaxBodySize( void )
{
	long		result;
	std::string	tmp;

	tmp = findInFileContent(_content, "client_max_body_size");
	if (tmp == "default")
		return (1000000);
	else
	{
		result = std::atol(tmp.c_str());
		std::string::iterator	it = tmp.end();
		it--;
		if (*it == 'G')
			return (result * 1000000000);
		else if (*it == 'M')
			return (result * 1000000);
		else
		{
			std::cout << "ERROR MAX BODY SIZE" << std::endl;
			_error = true;
			return (0);
		}
	}
}

/*================================ Accessors =================================*/

void	Config::setContent(std::vector<std::string> &src){
	_content = src;
}

void	Config::setPort(const std::vector<int> &src){
	_port = src;
}

void	Config::setHost(const std::vector<std::string> &src){
	_host = src;
}

void	Config::setName(const std::string &src){
	_name = src;
}

void	Config::setErrorPages(const std::map<std::string, std::string> &src){
	_errorPages = src;
}

void	Config::setMaxBodySize(const long &src){
	_maxBodySize = src;
}

void	Config::setLocations(const std::map<std::string, Location> &src)
{
	_locations = src;

	for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		it->second.setAllowedMethods(it->second.extractAllowedMethods());
		it->second.setRoot(it->second.extractRoot());
		it->second.setIndex(it->second.extractIndex());
		it->second.setListing(it->second.extractListing());
	}
}

std::vector<std::string> 			Config::getContent( void ){
	return (_content);
}

std::vector<int>					Config::getPort( void ){
	return (_port);
}

std::vector<std::string>			Config::getHost( void ){
	return (_host);
}

std::string							Config::getName( void ){
	return (_name);
}

std::map<std::string, std::string>	Config::getErrorPages( void ){
	return (_errorPages);
}

long								Config::getMaxBodySize( void ){
	return (_maxBodySize);
}

std::map<std::string, Location>		Config::getLocations( void ){
	return (_locations);
}
