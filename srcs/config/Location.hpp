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
	std::pair<int, std::string>	extractRedirection( void );
	std::string					extractRoot( void );
	bool						extractListing( void );
	std::vector<std::string>	extractIndex( void );
	std::vector<std::string>	extractAllowedCGI( void );

	/* Accessors */
	void	setContent( const std::vector<std::string> & );
	void	setPath( const std::string & );
	void	setError( const bool & );

	void	setAllowedMethods( const std::vector<std::string> & );
	void	setRedirection( const std::pair<int, std::string> & );
	void	setRoot( const std::string & );
	void	setListing( const bool & );
	void	setIndex( const std::vector<std::string> & );
	void	setAllowedCGI( const std::vector<std::string> & );

	std::vector<std::string>	getContent( void );
	std::string					getPath( void );
	bool						getError( void );

	std::vector<std::string>	getAllowedMethods( void );
	std::pair<int, std::string>	getRedirection( void );
	std::string					getRoot( void );
	bool						getListing( void );
	std::vector<std::string>	getIndex( void );
	std::vector<std::string>	getAllowedCGI( void );

	void	pushContent( const std::string & );
	bool	emptyContent( void );


	private :

	/* Attributes */
	std::vector<std::string>	_content;
	std::string					_path;
	bool						_error;

	std::vector<std::string>		_allowedMethods;	//allowed_method *
	std::pair<int, std::string>		_redirection;		//return *
	std::string						_root;				//root [path]
	bool							_listing;			//autoindex [on/off]
	std::vector<std::string>		_index;				//index *
	std::vector<std::string>		_allowedCGI;		//allowed_CGI *
};

#endif