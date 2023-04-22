#include "Location.hpp"

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
	return (*this);
}

/*================================= Methods ==================================*/



/*================================ Accessors =================================*/

void	Location::setPath( const std::string &src ){
	_path = src;
}

void	Location::setContent( const std::vector<std::string> &src ){
	_content = src;
}

std::string	Location::getPath( void ){
	return (_path);
}

std::vector<std::string> Location::getContent( void ){
	return (_content);
}
