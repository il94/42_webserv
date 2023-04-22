#ifndef LOCATION
#define LOCATION

#include <iostream>

#include <string>
#include <map>
#include <vector>

#include <algorithm>

class Location
{
	public :

	/* Constructors */
	Location();
	Location(const Location&);
	~Location();

	/* Overloads */
	Location&	operator=(const Location&);

	/* Methods */
	

	/* Accessors */
	void	setPath( const std::string & );
	void	setContent( const std::vector<std::string> & );

	std::string	getPath( void );
	std::vector<std::string>	getContent( void );

	private :

	/* Attributes */
	std::vector<std::string>			_content;
	std::string							_path;

	std::vector<std::string>	_allowedMethods; // allowed_method
	// HTTP redirection
	std::string	_root; //root [path]
	std::string	_index; //index [path]
	bool		_listing; //autoindex [on/off]
	std::string	_directoryRequest;	//error_directory_page [path_file.html]
	// std::vector<std::string>	_contentPath; //location...


};

#endif