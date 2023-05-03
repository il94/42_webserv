#include "Location.hpp"

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

	if (element.empty() == false)
	{
		std::string::iterator	it = element.begin();
		while (it != element.end() and (*it != ' ' and *it != '\t'))
			it++;

		if (it != element.end())
		{
			result.first = std::atoi(element.substr(0, std::distance(element.begin(), it)).c_str());

			while (it != element.end() and (*it == ' ' or *it == '\t'))
				it++;
			result.second = element.substr(std::distance(element.begin(), it), element.size());
		}
	}
	return (result);
}

std::string	Location::extractRoot( void )
{
	std::string	result;

	result = findInFileContent(_content, "root");
	if (result.empty())
		result = "/";
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
		result.push_back(getPath() + "index.html");
		result.push_back(getPath() + "index.php");
	}
	return (result);
}



std::vector<std::string>	Location::extractAllowedCGI( void )
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
	if (not src.empty() and src[0] != '/')
		_path = '/' + src;
	else
		_path = src;
}

void	Location::setError(const bool &src){
	_error = src;
}


void	Location::setAllowedMethods(const std::vector<std::string> &src)
{
	_allowedMethods.clear();

	std::vector<std::string>::const_iterator it = src.begin();

	for (it = src.begin(); it != src.end(); it++)
	{
		if (*it == "GET" or *it == "POST" or *it == "DELETE")
		{
			if (std::find(_allowedMethods.begin(), _allowedMethods.end(), *it) == _allowedMethods.end())
				_allowedMethods.push_back(*it);
		}
		else
		{
			setError(true);
			std::cout << *it << " is an invalid method" << std::endl;
		}
	}
}

void	Location::setRedirection(const std::pair<int, std::string> &src){
	_redirection = src;
}

void	Location::setRoot(const std::string &src)
{
	if (not src.empty() and src[0] != '/')
		_root = '/' + src;
	else
		_root = src;
}

void	Location::setIndex(const std::vector<std::string> &src)
{
	for (std::vector<std::string>::const_iterator it = src.begin(); it != src.end(); it++)
	{
		if (getPath().back() == '/')
			_index.push_back(getPath() + *it);
		else
			_index.push_back(getPath() + '/' + *it);
	}
}

void	Location::setListing(const bool &src){
	_listing = src;
}



void	Location::setAllowedCGI(const std::vector<std::string> &src)
{
	_index = src;
}



std::vector<std::string> 	Location::getContent( void ){
	return (_content);
}

std::string					Location::getPath( void ){
	return (_path);
}

bool				 		Location::getError( void ){
	return (_error);
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