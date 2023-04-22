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
	return (*this);
}

/*================================= Methods ==================================*/

void	Location::display( void )
{
	displayVector(getAllowedMethods(), "\tALLOWED METHODS");
	displayElement(getRoot(), "\tROOT");
	displayElement(getIndex(), "\tINDEX");
	displayElement(getListing(), "\tLISTING");
}

std::vector<std::string>	Location::extractAllowedMethods( void )
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent(_content, "allowed_method");

	if (result.empty() == true)
	{
		result.push_back("GET");
		result.push_back("POST");
		result.push_back("DELETE");
	}
	return (result);
}

std::string	Location::extractRoot( void )
{
	std::string	result;

	result = findInFileContent(_content, "root");
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

std::string	Location::extractIndex( void )
{
	std::string	result;

	result = findInFileContent(_content, "index");
	if (result == "default")
		return ("/index.html");
	else
	{
		if (result[0] != '/')
			result = '/' + result;
		return (result);
	}
}

bool	Location::extractListing( void )
{
	std::string	result;

	result = findInFileContent(_content, "autoindex");
	if (result == "on")
		return (true);
	return (false);
}

/*================================ Accessors =================================*/

void	Location::setContent( const std::vector<std::string> &src ){
	_content = src;
}

void	Location::setAllowedMethods(const std::vector<std::string> &src){
	_allowedMethods = src;
}

void	Location::setRoot(const std::string &src){
	_root = src;
}

void	Location::setIndex(const std::string &src){
	_index = src;
}

void	Location::setListing(const bool &src){
	_index = src;
}

std::vector<std::string> Location::getContent( void ){
	return (_content);
}

std::vector<std::string>	Location::getAllowedMethods( void ){
	return (_allowedMethods);
}

std::string	Location::getRoot( void ){
	return (_root);
}

std::string	Location::getIndex( void ){
	return (_index);
}

bool	Location::getListing( void ){
	return (_listing);
}



void	Location::pushContent( const std::string &src ){
	_content.push_back(src);
}

bool	Location::emptyContent( void ){
	return (_content.empty());
}