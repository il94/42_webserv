#include "Config.hpp"

#include "../utils/utils.hpp"

/*=============================== Constructors ===============================*/

Config::Config() : _error(false) {
	_port.push_back(DEFAULT_PORT);
	_host.push_back(DEFAULT_HOST);
	_name = DEFAULT_NAME;
	_maxBodySize = DEFAULT_MAX_BODY_SIZE;
	_errorPages.insert(std::pair<std::string, std::string>("400", "default_files/400.html"));
	_errorPages.insert(std::pair<std::string, std::string>("403", "default_files/403.html"));
	_errorPages.insert(std::pair<std::string, std::string>("404", "default_files/404.html"));
	_errorPages.insert(std::pair<std::string, std::string>("405", "default_files/405.html"));
	_errorPages.insert(std::pair<std::string, std::string>("413", "default_files/413.html"));
	_errorPages.insert(std::pair<std::string, std::string>("422", "default_files/422.html"));
	_errorPages.insert(std::pair<std::string, std::string>("500", "default_files/500.html"));
	_errorPages.insert(std::pair<std::string, std::string>("504", "default_files/504.html"));
	_errorPages.insert(std::pair<std::string, std::string>("default", "default_files/defaultError.html"));
	_errorPages.insert(std::pair<std::string, std::string>("default_404", "default_files/404.html"));
	_defaultLocation.setIndex("default_files/index.html");
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
	_maxBodySize = src._maxBodySize;
	_defaultLocation = src._defaultLocation;
	_errorPages = src._errorPages;
	_locations = src._locations;
	return (*this);
}

/*================================= Methods ==================================*/

void	Config::display( void )
{
	displayVector(getPort(), "PORT");
	displayVector(getHost(), "HOST");
	displayElement(getName(), "NAME");
	displayElement<unsigned long>(getMaxBodySize(), "MAX BODY SIZE");
	displayMap(getErrorPages(), "ERROR PAGE");

	_defaultLocation.display();

	std::cout << std::endl;
	for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		std::cout << "LOCATION [" << it->first << "]" << std::endl;
		it->second.display();
		std::cout << std::endl;
	}
}

std::vector<std::string>		Config::extractContent( std::vector<std::string> src )
{
	std::vector<std::string>	result;

	std::string::iterator		it;
	for (std::vector<std::string>::iterator start = src.begin(); start != src.end(); start++)
	{
		it = start->begin();
		while (it != start->end() && (*it == ' ' or *it == '\t'))
			it++;
		if (start->substr(std::distance(start->begin(), it), sizeof("location") - 1) == "location" and (*(it + sizeof("location") - 1) == ' ' or *(it + sizeof("location") - 1) == '\t'))
		{
			while (not closeBrace(*start, 1))
				start++;
		}
		else
			result.push_back(*start);
	}
	
	return (result);
}

std::vector<int>	Config::extractPort( void ) const
{
	std::vector<int>			result;
	std::vector<std::string>	content;

	content = multipleFindInFileContent(_content, "listen");
	if (content.empty())
	{
		result.push_back(DEFAULT_PORT);
		std::cerr << "[WARNING] Keyword \"listen\" not found. Default port setted (" << DEFAULT_PORT << ")" << std::endl;
	}
	else
	{
		for (std::vector<std::string>::iterator it = content.begin(); it < content.end(); it++)
			result.push_back(std::atoi((it->substr(it->find(':') + 1, it->size()).c_str())));
	}
	return (result);
}

std::vector<std::string>	Config::extractHost( void ) const
{
	std::vector<std::string>	result;
	std::vector<std::string>	content;

	content = multipleFindInFileContent(_content, "listen");
	if (content.empty() == true)
	{
		result.push_back(DEFAULT_HOST);
		std::cerr << "[WARNING] Keyword \"listen\" not found. Default host setted (" << DEFAULT_HOST << ")" << std::endl;
	}
	else
	{
		for (std::vector<std::string>::iterator it = content.begin(); it < content.end(); it++)
			result.push_back(it->substr(0, it->find(':')));
	}
	return (result);
}

std::string	Config::extractName( void ) const
{
	std::string	result;

	result = findInFileContent(_content, "server_name");

	if (result.empty())
	{
		result = DEFAULT_NAME;
		std::cerr << "[WARNING] Keyword \"server_name\" not found. Default name setted (" << DEFAULT_NAME << ")" << std::endl;
	}
	return (result);
}

unsigned long	Config::extractMaxBodySize( void ) const
{
	unsigned long	result;
	std::string		tmp;

	tmp = findInFileContent(_content, "client_max_body_size");
	if (tmp.empty())
	{
		result = DEFAULT_MAX_BODY_SIZE;
		std::cerr << "[WARNING] Keyword \"client_max_body_size\" not found. Default size setted (" << DEFAULT_MAX_BODY_SIZE << ")" << std::endl;
	}
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
		else if (it != tmp.end())
			std::cerr << "[WARNING] Unit not reconized. (excepted G, M, K or nothing)." << std::endl;
	}
	return (result);
}

std::map<std::string, std::string>	Config::extractErrorPages( void ) const
{
	std::map<std::string, std::string>	result;
	std::vector<std::string>			content;

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
			result.insert(element);
	}
	return (result);
}

Location	Config::extractDefaultLocation( void ) const
{
	Location	result;

	result.setContent(getContent());
	return (result);
}

std::map<std::string, Location>	Config::extractLocations( std::vector<std::string> src )
{
	std::map<std::string, Location>		result;

	std::vector<std::string>::iterator	start;

	std::string::iterator				it;

	for (start = src.begin(); start != src.end(); start++)
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
				if (element.first.front() != '/')
					element.first = '/' + element.first;
				if (element.first.back() != '/')
					element.first += '/';
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
			std::cerr << "[ERROR] " << *it << " Is an invalid port" << std::endl;
		}
	}
	if (getError() == false)
		_port = src;
}

void	Config::setHost(const std::vector<std::string> &src)
{
	std::vector<std::string>::const_iterator it = src.begin();

	for (it = src.begin(); getError() == false and it != src.end(); it++)
	{
		if (not isValidIP(*it))
		{
			setError(true);
			std::cerr << "[ERROR] " << *it << " Is an invalid host" << std::endl;
		}
	}
	if (getError() == false)
		_host = src;
}

void	Config::setName(const std::string &src){
	_name = src;
}

void	Config::setMaxBodySize(const unsigned long &src){
	_maxBodySize = src;
}

void	Config::setErrorPages(const std::map<std::string, std::string> &src)
{
	std::map<std::string, std::string>::const_iterator it = src.begin();

	for (it = src.begin(); it != src.end(); it++)
	{
		if (_errorPages.find(it->first) != _errorPages.end())
		{
			if (it->second[0] == '/')
				_errorPages[it->first] = getDefaultLocation().getRoot() + it->second.substr(1, it->second.size() - 1);
			else
				_errorPages[it->first] = getDefaultLocation().getRoot() + it->second;
		}
		else
			_errorPages.insert(*it);
	}
}

void	Config::setDefaultLocation( const Location &src )
{
	_defaultLocation.setRoot(src.extractRoot());
	_defaultLocation.setPath(_defaultLocation.getRoot());
	_defaultLocation.setIndex(src.extractIndex());
	_defaultLocation.setAllowedMethods(src.extractAllowedMethods());
	_defaultLocation.setListing(src.extractListing());
	_defaultLocation.setCGIBin(src.extractCGIBin());
	_defaultLocation.setAllowedCGI(src.extractAllowedCGI());
	_defaultLocation.setUploadPath(src.extractUploadPath());
	_defaultLocation.setRedirection(src.extractRedirection());
}

void	Config::setLocations(const std::map<std::string, Location> &src)
{
	_locations = src;

	for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		it->second = getDefaultLocationApplyContent(it->second.getContent());

		it->second.setPath(it->first);
		it->second.setRoot(it->second.extractRoot());
		it->second.setAllowedMethods(it->second.extractAllowedMethods());
		it->second.setRedirection(it->second.extractRedirection());
		it->second.setIndex(it->second.extractIndex());
		it->second.setListing(it->second.extractListing());
		it->second.setAllowedCGI(it->second.extractAllowedCGI());
		it->second.setCGIBin(it->second.extractCGIBin());
		it->second.setUploadPath(it->second.extractUploadPath());
	}
}

std::vector<std::string> 			Config::getContent( void ) const {
	return (_content);
}

bool				 				Config::getError( void ) const {
	return (_error);
}

std::vector<int>					Config::getPort( void ) const {
	return (_port);
}

std::vector<std::string>			Config::getHost( void ) const {
	return (_host);
}

std::string							Config::getName( void ) const {
	return (_name);
}

unsigned long						Config::getMaxBodySize( void ) const {
	return (_maxBodySize);
}

std::map<std::string, std::string>	Config::getErrorPages( void ) const {
	return (_errorPages);
}

std::string							Config::getErrorPages(const std::string &key) {
	return (_errorPages[key]);
}

Location							Config::getDefaultLocation( void ) const {
	return (_defaultLocation);
}

Location							Config::getDefaultLocationApplyContent( const std::vector<std::string> &content )
{
	Location	result(_defaultLocation);

	result.setContent(content);
	return (result);
}

std::map<std::string, Location>		Config::getLocations( void ) const {
	return (_locations);
}