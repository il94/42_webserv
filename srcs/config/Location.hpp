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

	std::vector<std::string>	extractAllowedMethods( void ) const;
	std::pair<int, std::string>	extractRedirection( void ) const;
	std::string					extractRoot( void ) const;
	std::string					extractListing( void ) const;
	std::vector<std::string>	extractIndex( void ) const;
	std::vector<std::string>	extractAllowedCGI( void ) const;
	std::vector<std::string>	extractCGIBin( void ) const;
	std::string					extractUploadPath( void ) const;

	/* Accessors */
	void	setContent( const std::vector<std::string> & );
	void	pushContent( const std::string & );
	bool	emptyContent( void );
	void	setPath( const std::string & );
	void	setError( const bool & );

	void	setAllowedMethods( const std::vector<std::string> & );
	void	setRedirection( const std::pair<int, std::string> & );
	void	setRoot( const std::string & );
	void	setListing( const bool & );
	void	setListing( const std::string & );
	void	setIndex( const std::vector<std::string> & );
	void	setAllowedCGI( const std::vector<std::string> & );
	void	setCGIBin( const std::vector<std::string> & );
	void	setUploadPath( const std::string & );

	std::vector<std::string>	getContent( void ) const;
	std::string					getPath( void ) const;
	bool						getError( void ) const;

	std::vector<std::string>	getAllowedMethods( void ) const;
	std::pair<int, std::string>	getRedirection( void ) const;
	std::string					getRoot( void ) const;
	bool						getListing( void ) const;
	std::vector<std::string>	getIndex( void ) const;
	std::vector<std::string>	getAllowedCGI( void ) const;
	std::vector<std::string>	getCGIBin( void ) const;
	std::string					getUploadPath( void ) const;



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
	std::vector<std::string>		_CGIBin;			//cgi_bin *
	std::string						_uploadPath;		//upload_path
};

#endif