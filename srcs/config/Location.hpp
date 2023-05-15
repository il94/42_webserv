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

	std::string					extractRoot( void ) const;
	std::vector<std::string>	extractIndex( void ) const;
	std::vector<std::string>	extractAllowedMethods( void ) const;
	std::string					extractListing( void ) const;
	std::vector<std::string>	extractCGIBin( void ) const;
	std::vector<std::string>	extractAllowedCGI( void ) const;
	std::string					extractUploadPath( void ) const;
	std::pair<int, std::string>	extractRedirection( void ) const;

	/* Accessors */
	void	setContent( const std::vector<std::string> & );
	void	pushContent( const std::string & );
	bool	emptyContent( void );
	void	setPath( const std::string & );

	void	setRoot( const std::string & );
	void	setIndex( const std::vector<std::string> & );
	void	setIndex( const std::string & );
	void	setAllowedMethods( const std::vector<std::string> & );
	void	setListing( const bool & );
	void	setListing( const std::string & );
	void	setCGIBin( const std::vector<std::string> & );
	void	setAllowedCGI( const std::vector<std::string> & );
	void	setUploadPath( const std::string & );
	void	setRedirection( const std::pair<int, std::string> & );

	std::vector<std::string>	getContent( void ) const;
	std::string					getPath( void ) const;

	std::string					getRoot( void ) const;
	std::vector<std::string>	getIndex( void ) const;
	std::vector<std::string>	getAllowedMethods( void ) const;
	bool						getListing( void ) const;
	std::vector<std::string>	getCGIBin( void ) const;
	std::vector<std::string>	getAllowedCGI( void ) const;
	std::string					getUploadPath( void ) const;
	std::pair<int, std::string>	getRedirection( void ) const;



	private :

	/* Attributes */
	std::vector<std::string>	_content;
	std::string					_path;

	std::string						_root;				//root
	std::vector<std::string>		_index;				//index *
	std::vector<std::string>		_allowedMethods;	//allowed_method *
	bool							_listing;			//autoindex
	std::vector<std::string>		_CGIBin;			//cgi_bin *
	std::vector<std::string>		_allowedCGI;		//allowed_CGI *
	std::string						_uploadPath;		//upload_path
	std::pair<int, std::string>		_redirection;		//return
};

#endif