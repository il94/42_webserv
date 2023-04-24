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
	// displayMap(getErrorPages(), "ERROR PAGE");
	// displayElement(getMaxBodySize(), "MAX BODY SIZE");

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
				element.second.setPath(element.first);
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

std::vector<int>	Config::extractPort( void )
{
	std::vector<int>			result;
	std::vector<std::string>	content;

	content = multipleFindInFileContent(_content, "listen");
	if (content.empty())
		result.push_back(DEFAULT_PORT);
	else
	{
		unsigned int	tmp;

		for (std::vector<std::string>::iterator it = content.begin(); it < content.end(); it++)
		{
			tmp = std::atoi((it->substr(it->find(':') + 1, it->size()).c_str()));
			if (tmp >= 1024)
				result.push_back(tmp);
		}
	}
	return (result);
}

std::vector<std::string>	Config::extractHost( void ) //verif si IP valide + format
{
	std::vector<std::string>	result;
	std::vector<std::string>	content;

	content = multipleFindInFileContent(_content, "listen");
	if (content.empty() == true)
		result.push_back(DEFAULT_HOST);
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
	if (result.empty())
		result = DEFAULT_NAME;
	return (result);
}

std::map<std::string, std::string>	Config::extractErrorPages( void ) //ajouter toutes les pages necessaires
{
	std::map<std::string, std::string>	result;
	std::vector<std::string>			content;

	result.insert(std::pair<std::string, std::string>("404", "/default_files/404.html"));
	result.insert(std::pair<std::string, std::string>("500", "/default_files/500.html"));
	result.insert(std::pair<std::string, std::string>("501", "/default_files/501.html"));
	result.insert(std::pair<std::string, std::string>("502", "/default_files/502.html"));

	content = multipleFindInFileContent(_content, "error_page");

	std::pair<std::string, std::string>	element;
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); *it++)
	{
		if (result.find(it->substr(0, it->find(' '))) != result.end())
			result[it->substr(0, it->find(' '))] = it->substr(it->find(' ') + 1, ';');
	}
	return (result);
}

long	Config::extractMaxBodySize( void )
{
	long		result;
	std::string	tmp;

	tmp = findInFileContent(_content, "client_max_body_size");
	if (tmp.empty())
		result = 1000000;
	else
	{
		result = std::atol(tmp.c_str());
		std::string::iterator	it = tmp.end();
		it--;
		if (*it == 'G')
			result *= 1000000000;
		else if (*it == 'M')
			result *= 1000000;
		else
		{
			std::cout << "ERROR MAX BODY SIZE" << std::endl;
			_error = true;
			result = 0;
		}
	}
	return (result);
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
		it->second.setRedirection(it->second.extractRedirection());
		it->second.setRoot(it->second.extractRoot());
		it->second.setIndex(it->second.extractIndex());
		it->second.setListing(it->second.extractListing());
		it->second.setAllowedCGI(it->second.extractAllowedCGI());
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

size_t								Config::getMaxBodySize( void ){
	return (_maxBodySize);
}

std::map<std::string, Location>		Config::getLocations( void ){
	return (_locations);
}

std::string	Config::getErrorPages(std::string key){
	return (_errorPages[key]);
}


#include "../utils/utils.hpp"

/*=============================== Constructors ===============================*/

Location::Location(){
}

Location::Location(const Location &src){
	*this = src;
}

Location::~Location(){
}

/*================================ Overloads =================================*/

Location& Location::operator=(const Location &src)
{
	_content = src._content;
	_path = src._path;

	_allowedMethods = src._allowedMethods;
	_redirection = src._redirection;
	_root = src._root;
	_index = src._index;
	_listing = src._listing;
	return (*this);
}

/*================================= Methods ==================================*/

void	Location::display( void )
{
	displayVector(getAllowedMethods(), "\tALLOWED METHODS");
	displayPair(getRedirection(), "\tREDIRECTION");
	displayElement(getRoot(), "\tROOT");
	displayVector(getIndex(), "\tINDEX");
	displayElement(getListing(), "\tLISTING");
	displayVector(getAllowedCGI(), "\tALLOWED CGI");
}

std::vector<std::string>	Location::extractAllowedMethods( void ) //verifier methodes invalides
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent(_content, "allowed_method");

	if (result.empty())
	{
		result.push_back("GET");
		result.push_back("POST");
		result.push_back("DELETE");
	}
	return (result);
}

std::pair<int, std::string>	Location::extractRedirection( void )
{
	std::pair<int, std::string>	result;
	std::string					element;

	element = findInFileContent(_content, "return");

	if (element.empty())
	{
		result.first = 301;
		// result.second = ???
	}
	else
	{
		if (element.find(element.substr(0, element.find(' '))) != std::string::npos or element.find(element.substr(0, element.find('\t'))) != std::string::npos)
		{
			result.first = std::atoi(element.substr(0, element.find(' ')).c_str());
			result.second = element.substr(element.rfind(' ') + 1);
		}
		else
		{
			// ERROR
		}
	}
	return (result);
}

std::string	Location::extractRoot( void )
{
	std::string	result;

	result = findInFileContent(_content, "root");
	if (result.empty())
		result = getPath();
	return (result);
}

bool	Location::extractListing( void )
{
	std::string	result;

	result = findInFileContent(_content, "autoindex");
	return (result.empty() or result == "on");
}

std::vector<std::string>	Location::extractIndex( void )
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent(_content, "index");
	if (result.empty())
	{
		if (getPath().back() == '/')
			result.push_back(getPath() + "index.html");
		else
			result.push_back(getPath() + '/' + "index.html");
	}
	else
	{
		for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); it++)
		{
			if (getPath().back() == '/')
				it->insert(0, getPath());
			else
				it->insert(0, getPath() + '/');
		}
	}

	return (result);
}



std::vector<std::string>	Location::extractAllowedCGI( void ) //verifier methodes invalides
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent(_content, "allowed_CGI");

	if (result.empty())
	{
		result.push_back(".py");
		result.push_back(".php");
	}
	return (result);
}

/*================================ Accessors =================================*/

void	Location::setContent(const std::vector<std::string> &src){
	_content = src;
}

void	Location::setPath( const std::string &src ){
	_path = src;
}



void	Location::setAllowedMethods(const std::vector<std::string> &src){
	_allowedMethods = src;
}

void	Location::setRedirection(const std::pair<int, std::string> &src){
	_redirection = src;
}

void	Location::setRoot(const std::string &src){
	_root = src;
}

void	Location::setIndex(const std::vector<std::string> &src){
	_index = src;
}

void	Location::setListing(const bool &src){
	_listing = src;
}



void	Location::setAllowedCGI(const std::vector<std::string> &src){
	_allowedCGI = src;
}



std::vector<std::string> 	Location::getContent( void ){
	return (_content);
}

std::string					Location::getPath( void ){
	return (_path);
}



std::vector<std::string>	Location::getAllowedMethods( void ){
	return (_allowedMethods);
}

std::pair<int, std::string>	Location::getRedirection( void ){
	return (_redirection);
}

std::string					Location::getRoot( void ){
	return (_root);
}

std::vector<std::string>	Location::getIndex( void ){
	return (_index);
}

bool						Location::getListing( void ){
	return (_listing);
}



std::vector<std::string>	Location::getAllowedCGI( void ){
	return (_allowedCGI);
}



void	Location::pushContent( const std::string &src ){
	_content.push_back(src);
}

bool	Location::emptyContent( void ){
	return (_content.empty());
}