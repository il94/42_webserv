#include "Config.hpp"

#include "../utils/utils.hpp"

/*=============================== Constructors ===============================*/

Config::Config(){
	_error = false;
	_port.push_back(DEFAULT_PORT);
	_host.push_back(DEFAULT_HOST);
	_name = DEFAULT_NAME;
	_errorPages.insert(std::pair<std::string, std::string>("404", "/default_files/404.html"));
	_errorPages.insert(std::pair<std::string, std::string>("500", "/default_files/500.html"));
	_errorPages.insert(std::pair<std::string, std::string>("501", "/default_files/501.html"));
	_errorPages.insert(std::pair<std::string, std::string>("502", "/default_files/502.html"));
	_maxBodySize = DEFAULT_MAX_BODY_SIZE;
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

	_route.display();

	std::cout << std::endl;
	for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		std::cout << "LOCATION [" << it->first << "]" << std::endl;
		it->second.display();
		std::cout << std::endl;
	}
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
		if (start->substr(std::distance(start->begin(), it), sizeof("location") - 1) == "location" and (*(it + sizeof("location") - 1) == ' ' or *(it + sizeof("location") - 1) == '\t'))
		{
			it += sizeof("location");
			while (*it == ' ' or *it == '\t')
				it++;
			if (it != start->end())
			{
				std::pair<std::string, Location>	element;
				while (*it != ' ' and *it != '\t')
				{
					element.first += *it;
					it++;
				}
				element.second.setPath(element.first);

				if (openBrace(*start, std::distance(start->begin(), it + 1)))
				{
					start++;
					while (not closeBrace(*start, 1) and static_cast<int>(start->find("location")) == -1)
					{
						element.second.pushContent(*start);
						start++;
					}
					if (not element.second.emptyContent() and static_cast<int>(start->find("location")) == -1)
						result.insert(std::pair<std::string, Location>(element));
					else if (static_cast<int>(start->find("location")) != -1)
						start--;
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

std::map<std::string, std::string>	Config::extractErrorPages( void )
{
	std::map<std::string, std::string>	result;
	std::vector<std::string>			content;

	result = _errorPages;
	content = multipleFindInFileContent(_content, "error_page");

	std::pair<std::string, std::string>	element;
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); *it++)
	{
		std::string::iterator	i = it->begin();

		while (i != it->end() and (*i != ' ' and *i != '\t'))
			i++;
		element.first = it->substr(0, std::distance(it->begin(), i));
		while (i != it->end() and (*i == ' ' or *i == '\t'))
			i++;
		
		element.second = it->substr(std::distance(it->begin(), i), it->size());

		if (element.first.empty() == false and element.second.empty() == false)
		{
			if (result.find(element.first) != result.end())
			{
				result[element.first] = element.second;
				if (result[element.first][0] != '/')
					result[element.first] = "/" + result[element.first];
			}
			else
			{
				if (element.second[0] != '/')
					element.second = "/" + element.second;
				result.insert(element);
			}
		}
	}
	return (result);
}

unsigned long	Config::extractMaxBodySize( void )
{
	unsigned long	result;
	std::string		tmp;

	tmp = findInFileContent(_content, "client_max_body_size");
	if (tmp.empty())
		result = DEFAULT_MAX_BODY_SIZE;
	else
	{
		result = std::atol(tmp.c_str());

		std::string::iterator it;

		for (it = tmp.begin(); std::isdigit(*it) and it != tmp.end(); it++)
		{
		}

		if (*it == 'G')
			result *= 1000000000;
		else if (*it == 'M')
			result *= 1000000;
		else if (*it == 'K')
			result *= 1000;
	}
	return (result);
}

Location	Config::extractRoute( void )
{
	Location	result;

	result.setContent(getContent());
	return (result);
}

/*================================ Accessors =================================*/

void	Config::setContent(const std::vector<std::string> &src){
	_content = src;
}

void	Config::setError(const bool &src){
	_error = src;
}

void	Config::setPort(const std::vector<int> &src)
{
	std::vector<int>::const_iterator it = src.begin();

	for (it = src.begin(); getError() == false and it != src.end(); it++)
	{
		if (*it < 1024)
		{
			setError(true);
			std::cout << "[ERROR] " << *it << " Is an invalid port" << std::endl;
		}
	}
	if (getError() == false)
		_port = src;
}

void	Config::setHost(const std::vector<std::string> &src)//verif si IP valide + format
{
	std::vector<std::string>::const_iterator it = src.begin();

	for (it = src.begin(); getError() == false and it != src.end(); it++)
	{
		if (not isValidIP(*it))
		{
			setError(true);
			std::cout << "[ERROR] " << *it << " Is an invalid host" << std::endl;
		}
	}
	if (getError() == false)
		_host = src;
}

void	Config::setName(const std::string &src){ //verif si nom deja pris
	_name = src;
}

void	Config::setErrorPages(const std::map<std::string, std::string> &src){
	_errorPages = src;
}

void	Config::setMaxBodySize(const unsigned long &src){ //definir max ??
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
		// if (it->second.getError() == true)
		// {
		// 	_locations.erase(it);

		// }
	}
}

void	Config::setRoute( const Location &src )
{
	_route.setAllowedMethods(_route.extractAllowedMethods());
	_route.setRedirection(_route.extractRedirection());
	_route.setRoot(_route.extractRoot());
	_route.setIndex(_route.extractIndex());
	_route.setListing(_route.extractListing());
	_route.setAllowedCGI(_route.extractAllowedCGI());
}

std::vector<std::string> 			Config::getContent( void ){
	return (_content);
}

bool				 				Config::getError( void ){
	return (_error);
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

std::string	Config::getErrorPages(const std::string &key){
	return (_errorPages[key]);
}

unsigned long						Config::getMaxBodySize( void ){
	return (_maxBodySize);
}

std::map<std::string, Location>		Config::getLocations( void ){
	return (_locations);
}

Location							Config::getRoute( void ){
	return (_route);
}
