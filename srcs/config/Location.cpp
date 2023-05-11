#include "Location.hpp"

#include "../utils/utils.hpp"

/*=============================== Constructors ===============================*/

Location::Location() : _error(false), _listing(false){
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
	_error = src._error;

	_allowedMethods = src._allowedMethods;
	_redirection = src._redirection;
	_root = src._root;
	_listing = src._listing;
	_index = src._index;
	_allowedCGI = src._allowedCGI;
	_CGIBin = src._CGIBin;
	_uploadPath = src._uploadPath;
	return (*this);
}

/*================================= Methods ==================================*/

void	Location::display( void )
{
	displayVector(getAllowedMethods(), "\tALLOWED METHODS");
	displayPair(getRedirection(), "\tREDIRECTION");
	displayElement(getRoot(), "\tROOT");
	displayVector(getIndex(), "\tINDEX");
	displayElement<bool>(getListing(), "\tLISTING");
	displayVector(getAllowedCGI(), "\tALLOWED CGI");
	displayVector(getCGIBin(), "\tCGI BIN");
	displayElement(getUploadPath(), "\tUPLOAD PATH");
}

std::vector<std::string>	Location::extractAllowedMethods( void ) const //verifier methodes invalides
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent(_content, "allowed_method");
	return (result);
}

std::pair<int, std::string>	Location::extractRedirection( void ) const
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

std::string	Location::extractRoot( void ) const
{
	std::string	result;

	result = findInFileContent(_content, "root");
	return (result);
}

std::string	Location::extractListing( void ) const
{
	std::string	result;

	result = findInFileContent(_content, "autoindex");
	return (result);
}

std::vector<std::string>	Location::extractIndex( void ) const
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent(_content, "index");
	return (result);
}

std::vector<std::string>	Location::extractAllowedCGI( void ) const
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent(_content, "allowed_CGI");
	return (result);
}

std::vector<std::string>	Location::extractCGIBin( void ) const
{
	std::vector<std::string>	result;

	result = multipleFindInFileContent(_content, "CGI_bin");
	return (result);
}

std::string	Location::extractUploadPath( void ) const
{
	std::string	result;

	result = findInFileContent(_content, "upload_path");
	return (result);
}

/*================================ Accessors =================================*/

void	Location::setContent(const std::vector<std::string> &src){
	_content = src;
}

void	Location::pushContent( const std::string &src ){
	_content.push_back(src);
}

bool	Location::emptyContent( void ){
	return (_content.empty());
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

void	Location::setRedirection(const std::pair<int, std::string> &src)
{
	if (src.second.empty() == false)
		_redirection = src;
}

void	Location::setRoot(const std::string &src)
{
	if (src.empty() == false)
	{
		if (src[0] != '/')
			_root = '/' + src;
		else
			_root = src;
		if (_root.back() != '/')
			_root += '/';
	}

}

void	Location::setIndex(const std::vector<std::string> &src)
{
	if (src.empty() == false)
	{
		_index.clear();

		for (std::vector<std::string>::const_iterator it = src.begin(); it != src.end(); it++)
		{
			if ((*it)[0] == '/')
				_index.push_back(getRoot() + it->substr(1, it->size() - 1));
			else
				_index.push_back(getRoot() + *it);
		}
	}
}

void	Location::setListing(const bool &src){
	_listing = src;
}

void	Location::setListing(const std::string &src)
{
	if (src.empty() == false)
	{
		if (src == "on")
			_listing = true;
		else if (src == "off")
			_listing = false;
	}
}

void	Location::setAllowedCGI(const std::vector<std::string> &src)
{
	if (src.empty() == false)
	{
		for (std::vector<std::string>::const_iterator it = src.begin(); it != src.end(); it++)
		{
			if (it->empty() == false and (*it)[0] == '.')
				_allowedCGI.push_back(*it);
		}
	}
}

void	Location::setCGIBin(const std::vector<std::string> &src)
{
	if (src.empty() == false)
	{
		std::string	tmp;
		for (std::vector<std::string>::const_iterator it = src.begin(); it != src.end(); it++)
		{
			tmp.clear();

			if ((*it)[0] == '/')
				tmp = getRoot() + it->substr(1, it->size() - 1);
			else
				tmp = getRoot() + *it;
			if (tmp.back() != '/')
				tmp += '/';
			_CGIBin.push_back(tmp);
		}
	}
}

void	Location::setUploadPath(const std::string &src)
{
	if (src.empty() == false)
	{
		_uploadPath.clear();

		if (src[0] == '/')
			_uploadPath = getRoot() + src.substr(1, src.size() - 1);
		else
			_uploadPath = getRoot() + src;
		if (_uploadPath.back() != '/')
			_uploadPath += '/';
	}
}

std::vector<std::string> 	Location::getContent( void ) const {
	return (_content);
}

std::string					Location::getPath( void ) const {
	return (_path);
}

bool				 		Location::getError( void ) const {
	return (_error);
}

std::vector<std::string>	Location::getAllowedMethods( void ) const {
	return (_allowedMethods);
}

std::pair<int, std::string>	Location::getRedirection( void ) const {
	return (_redirection);
}

std::string					Location::getRoot( void ) const {
	return (_root);
}

std::vector<std::string>	Location::getIndex( void ) const {
	return (_index);
}

bool						Location::getListing( void ) const {
	return (_listing);
}

std::vector<std::string>	Location::getAllowedCGI( void ) const {
	return (_allowedCGI);
}

std::vector<std::string>	Location::getCGIBin( void ) const {
	return (_CGIBin);
}

std::string					Location::getUploadPath( void ) const {
	return (_uploadPath);
}
