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
	void	display( void );

	std::vector<std::string>	extractAllowedMethods( void );
	std::string					extractRoot( void );
	std::string					extractIndex( void );
	bool						extractListing( void );

	/* Accessors */
	void	setContent( const std::vector<std::string> & );
	void	setPath( const std::string & );
	void	setAllowedMethods( const std::vector<std::string> & );
	void	setRoot( const std::string & );
	void	setIndex( const std::string & );
	void	setListing( const bool & );

	std::vector<std::string>	getContent( void );
	std::string					getPath( void );
	std::vector<std::string>	getAllowedMethods( void );
	std::string					getRoot( void );
	std::string					getIndex( void );
	bool						getListing( void );

	void	pushContent( const std::string & );
	bool	emptyContent( void );



	private :

	/* Attributes */
	std::vector<std::string>	_content;

	std::vector<std::string>	_allowedMethods;	// allowed_method
	// HTTP redirection
	std::string					_root;				//root [path]
	std::string					_index;				//index [path]
	bool						_listing;			//autoindex [on/off]
	std::string					_directoryRequest;	//error_directory_page [path_file.html]
	// std::vector<std::string>	_contentPath;		//location...


};

#endif