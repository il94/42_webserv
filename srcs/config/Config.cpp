#include "Config.hpp"

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
	_fileContent = src._fileContent;
	_port = src._port;
	_host = src._host;
	_name = src._name;
	_errorPages = src._errorPages;
	_maxBodySize = src._maxBodySize;
	_allowedMethods = src._allowedMethods;
	return (*this);
}

/*================================= Methods ==================================*/

std::string	Config::findInFileContent(const std::string &src)
{
	std::string	result;
	int			index = -1;
	size_t		i = 0;

	for (i = 0; i < _fileContent.size() and index == -1 ; i++)
		index = _fileContent[i].find(src);
	if (index == -1)
		return ("default");

	result = _fileContent[i - 1].substr(index + src.size() + 1, _fileContent[i - 1].find(';') - (index + src.size() + 1));
	return (result);
}

std::vector<std::string>	Config::multipleFindInFileContent(const std::string &src)
{
	std::vector<std::string>	result;
	int							index = -1;

	for (size_t i = 0; i < _fileContent.size(); i++)
	{
		index = _fileContent[i].find(src);
		if (index != -1)
			result.push_back(_fileContent[i].substr(index + src.size() + 1, _fileContent[i].find(';') - (index + src.size() + 1)));
	}
	return (result);
}

void	Config::printConfig( void )
{
	std::cout << "PORT = " << getPort() << std::endl;
	std::cout << "HOST = " << getHost() << std::endl;
	std::cout << "NAME = " << getName() << std::endl;
	for (std::map<std::string, std::string>::iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
		std::cout << "ERROR PAGE " << it->first << " = " << it->second << std::endl;
	std::cout << "MAX BODY SIZE = " << getMaxBodySize() << std::endl;

	// for (size_t i = 0; i < getLocations().size(); i++)
	// {
	// 	std::cout << "LOCATION " << _locations[i].getPath() << std::endl;
	// 	// for (Location::iterator it = _locations[i].begin(); it != _locations[i].end(); it++)
	// 	// {
	// 	// 	std::cout << *it << std::endl;
	// 	// }
	// }
	

	for (std::vector<std::string>::iterator it = _allowedMethods.begin(); it != _allowedMethods.end(); it++)
		std::cout << "ALLOWED METHOD = " << *it << std::endl;
	std::cout << "ROOT = " << getRoot() << std::endl;
	std::cout << "INDEX = " << getIndex() << std::endl;
	std::cout << "LISTING = " << getListing() << std::endl;
}

bool	closeBrace(std::string src)
{
	std::string::iterator it = src.begin();

	while (it != src.end() and (*it == ' ' or *it == '\t'))
		it++;
	return (*it == '}');
}

// std::vector<Location>	Config::extractLocations( void )
// {
// 	std::vector<Location>	result;
// 	Location				element;

// 	std::vector<std::string>::iterator	start;
// 	std::vector<std::string>::iterator	end;

// 	std::string::iterator	it;

// 	for (start = _fileContent.begin(); start != _fileContent.end(); start++)
// 	{
// 		it = start->begin();
// 		element.clear();
// 		while (*it == ' ' or *it == '\t')
// 			it++;
// 		if (start->substr(std::distance(start->begin(), it), sizeof("location")) == "location ")
// 		{
// 			it += sizeof("location");
// 			while (*it == ' ' or *it == '\t')
// 				it++;
// 			if (it != start->end())
// 			{
// 				element.setPath( start->substr(std::distance(start->begin(), it), start->find(' ', std::distance(start->begin(), it + 1) - std::distance(start->begin(), it) )));
// 				it += element.getPath().size();
// 				if (start->find("{\n", start->find(' ', std::distance(start->begin(), it + 1) - std::distance(start->begin(), it))))
// 				{
// 					start++;
// 					while (not closeBrace(*start))
// 					{
// 						element.push_back(*start);
// 						start++;
// 					}
// 					if (element.empty() == false)
// 						result.push_back(element);
// 					std::cout << "PRINT = " <<   element.getPath() << std::endl;
// 					std::cout << "PRINT = " <<   element[0] << std::endl;
// 					std::cout << "PRINT2 = " <<   result[0].getPath() << std::endl;
// 					std::cout << "PRINT2 = " <<   result[0][0] << std::endl;
// 				}
// 			}
// 		}
// 	}
// 	return (result);
// }

// std::vector<Location>	Config::extractLocations( void )
// {

// 	std::vector<Location>	result;
// 	Location				element;


// 	std::vector<std::string>::iterator	start;
// 	std::vector<std::string>::iterator	end;

// 	std::string::iterator	it;

// 	for (start = _fileContent.begin(); start != _fileContent.end(); start++)
// 	{
// 		it = start->begin();
// 		element.clear();
// 		while (*it == ' ' or *it == '\t')
// 			it++;
// 		if (start->substr(std::distance(start->begin(), it), sizeof("location")) == "location ")
// 		{
// 			it += sizeof("location");
// 			while (*it == ' ' or *it == '\t')
// 				it++;
// 			if (it != start->end())
// 			{
// 				element.setPath( start->substr(std::distance(start->begin(), it), start->find(' ', std::distance(start->begin(), it + 1) - std::distance(start->begin(), it) )));
// 				it += element.getPath().size();
// 				if (start->find("{\n", start->find(' ', std::distance(start->begin(), it + 1) - std::distance(start->begin(), it))))
// 				{
// 					start++;
// 					while (not closeBrace(*start))
// 					{
// 						element.push_back(*start);
// 						start++;
// 					}
// 					if (element.empty() == false)
// 						result.push_back(element);
// 					std::cout << "PRINT = " <<   element.getPath() << std::endl;
// 					std::cout << "PRINT = " <<   element[0] << std::endl;
// 					std::cout << "PRINT2 = " <<   result[0].getPath() << std::endl;
// 					std::cout << "PRINT2 = " <<   result[0][0] << std::endl;
// 				}
// 			}
// 		}
// 	}
// 	return (result);
// }


short	Config::extractPort( void )
{
	std::string	result;

	result = findInFileContent("listen");
	if (result == "default")
		return (8000);
	else
		return (std::atoi((result.substr(result.find(':') + 1, result.size()).c_str())));
}

std::string	Config::extractHost( void )
{
	std::string	result;

	result = findInFileContent("listen");
	if (result == "default")
		return ("127.0.0.1");
	else
		return (result.substr(0, result.find(':')));
}

std::string	Config::extractName( void )
{
	std::string	result;

	result = findInFileContent("server_name");
	if (result == "default")
		return ("default_server");
	else
		return (result);
}

std::map<std::string, std::string>	Config::extractErrorPages( void )
{
	std::map<std::string, std::string>	result;
	std::vector<std::string>			content;

	content = multipleFindInFileContent("error_page");

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

	tmp = findInFileContent("client_max_body_size");
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

std::vector<std::string>	Config::extractAllowedMethods( void )
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent("allowed_method");

	if (result.empty() == true)
	{
		result.push_back("GET");
		result.push_back("POST");
		result.push_back("DELETE");
	}
	return (result);
}

std::string	Config::extractRoot( void )
{
	std::string	result;

	result = findInFileContent("root");
	if (result == "default")
	{
		_listing = true;
		result = "/";
	}
	else
	{
		if (result[0] != '/')
			result = '/' + result;
	}
	return (result);
}

std::string	Config::extractIndex( void )
{
	std::string	result;

	result = findInFileContent("index");
	if (result == "default")
		return ("/index.html");
	else
	{
		if (result[0] != '/')
			result = '/' + result;
		return (result);
	}
}

bool	Config::extractListing( void )
{
	std::string	result;

	result = findInFileContent("autoindex");
	if (result == "on")
		return (true);
	return (false);
}

/*================================ Accessors =================================*/

void	Config::setFileContent(std::vector<std::string> &src){
	_fileContent = src;
}

void	Config::setPort(const short &src){
	_port = src;
}

void	Config::setHost(const std::string &src){
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

void	Config::setLocations(const std::vector<Location> &src){
	_locations = src;
}

void	Config::setAllowedMethods(const std::vector<std::string> &src){
	_allowedMethods = src;
}

void	Config::setRoot(const std::string &src){
	_root = src;
}

void	Config::setIndex(const std::string &src){
	_index = src;
}

void	Config::setListing(const bool &src){
	_index = src;
}





short	Config::getPort( void ){
	return (_port);
}

std::string	Config::getHost( void ){
	return (_host);
}

std::string	Config::getName( void ){
	return (_name);
}

std::map<std::string, std::string>	Config::getErrorPages( void ){
	return (_errorPages);
}

long		Config::getMaxBodySize( void ){
	return (_maxBodySize);
}

std::vector<Location>	Config::getLocations( void ){
	return (_locations);
}

std::vector<std::string>	Config::getAllowedMethods( void ){
	return (_allowedMethods);
}

std::string	Config::getRoot( void ){
	return (_root);
}

std::string	Config::getIndex( void ){
	return (_index);
}

bool	Config::getListing( void ){
	return (_listing);
}
