#ifndef CONFIG
#define CONFIG

#include <iostream>

#include <string>
#include <map>
#include <vector>

#include <algorithm>

#include "Location.hpp"
#include "../utils/utils.hpp"

#define DEFAULT_PORT 8000
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_NAME "default_server"
#define DEFAULT_MAX_BODY_SIZE 1000000

class Config
{
	public :

	/* Constructors */
	Config();
	Config(const Config&);
	~Config();

	/* Overloads */
	Config&	operator=(const Config&);

	/* Methods */

	void								display( void );

	std::vector<std::string>			extractContent( std::vector<std::string> );

	std::vector<int>					extractPort ( void ) const ;
	std::vector<std::string>			extractHost( void ) const ;
	std::string							extractName( void ) const ;
	unsigned long						extractMaxBodySize( void ) const ;
	std::map<std::string, std::string>	extractErrorPages( void ) const ;

	Location							extractDefaultLocation( void ) const ;
	std::map<std::string, Location>		extractLocations( std::vector<std::string> src ) ;

	/* Accessors */
	void	setContent( const std::vector<std::string> & );
	void	setError( const bool & );

	void	setPort( const std::vector<int> & );
	void	setHost( const std::vector<std::string> & );
	void	setName( const std::string & );
	void	setMaxBodySize( const unsigned long & );
	void	setErrorPages( const std::map<std::string, std::string> & );

	void	setDefaultLocation( const Location & );
	void	setLocations( const std::map<std::string, Location> & );


	std::vector<std::string>			getContent( void ) const ;
	bool								getError( void ) const ;

	std::vector<int>					getPort( void ) const ;
	std::vector<std::string>			getHost( void ) const ;
	std::string							getName( void ) const ;
	unsigned long						getMaxBodySize( void ) const ;
	std::map<std::string, std::string>	getErrorPages( void ) const ;
	std::string							getErrorPages( const std::string & ) ;

	Location							getDefaultLocation( void ) const ;
	Location							getDefaultLocationApplyContent( const std::vector<std::string> & );
	std::map<std::string, Location>		getLocations( void ) const ;

	private :

	/* Attributes */
	std::vector<std::string>	_content;
	bool						_error;

	std::vector<int>					_port;			//listen *
	std::vector<std::string>			_host;			//listen *
	std::string							_name;			//server_name
	long 								_maxBodySize;	//client_max_body_size
	std::map<std::string, std::string>	_errorPages;	//error_page *

	Location						_defaultLocation;
	std::map<std::string, Location>	_locations;
};

#endif