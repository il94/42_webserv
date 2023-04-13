#include "Config.hpp"

#include <algorithm>

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
	// config = src.config;
	return (*this);
}

/*================================= Methods ==================================*/

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

void	Config::printConfig( void )
{
	std::cout << "PORT = " << getPort() << std::endl;
	std::cout << "HOST = " << getHost() << std::endl;
	std::cout << "SOCKET = " << getSocket() << std::endl;
	std::cout << "NAME = " << getName() << std::endl;
	for (std::map<std::string, std::string>::iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
		std::cout << "ERROR PAGE " << it->first << " = " << it->second << std::endl;
	std::cout << "MAX BODY SIZE  = " << getMaxBodySize() << std::endl;
}




std::string	Config::extractPort( void )
{
	std::string	result;

	result = findInFileContent("listen");
	if (result == "default")
		return ("8000");
	else
		return (result.substr(result.find(':') + 1, result.size()));
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

std::string	Config::extractSocket( void )
{
	std::string	result;

	result = findInFileContent("listen");
	if (result == "default")
		return ("127.0.0.1:8080");
	else
		return (result);
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
	std::vector<std::string>	content;

	content = multipleFindInFileContent("error_page");

	if (content.empty() == true)
		result.insert(std::pair<std::string, std::string>("404", "404.html"));
	else
	{
		std::pair<std::string, std::string>	element;
		for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); *it++)
		{
			element.first = it->substr(0, it->find('/') - 1);
			element.second = it->substr(it->find('/'), ';');
			result.insert(element);
		}
	}
	return (result);
}

std::string	Config::extractMaxBodySize( void )
{
	std::string	result;

	result = findInFileContent("client_max_body_size");
	if (result == "default")
		return ("1M");
	else
		return (result);
}

/*================================ Accessors =================================*/

void	Config::setFileContent(std::vector<std::string> &src){
	_fileContent = src;
}

void	Config::setPort(const std::string &src){
	_port = src;
}

void	Config::setHost(const std::string &src){
	_host = src;
}

void	Config::setSocket(const std::string &src){
	_socket = src;
}

void	Config::setName(const std::string &src){
	_name = src;
}

void	Config::setErrorPages(const std::map<std::string, std::string> &src){
	_errorPages = src;
}

void	Config::setMaxBodySize(const std::string &src){
	_maxBodySize = src;
}



std::string	Config::getPort( void ){
	return (_port);
}

std::string	Config::getHost( void ){
	return (_host);
}

std::string	Config::getSocket( void ){
	return (_socket);
}

std::string	Config::getName( void ){
	return (_name);
}

std::map<std::string, std::string>	Config::getErrorPages( void ){
	return (_errorPages);
}

std::string	Config::getMaxBodySize( void ){
	return (_maxBodySize);
}
