#ifndef CONFIG
#define CONFIG

# include "../../include/webserv.hpp"

#define DEFAULT_PORT 8000
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_NAME "default_server"
#define DEFAULT_MAX_BODY_SIZE 1000000


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

	void	setAllowedMethods( const std::vector<std::string> & );
	void	setRedirection( const std::pair<int, std::string> & );
	void	setRoot( const std::string & );
	void	setListing( const bool & );
	void	setIndex( const std::vector<std::string> & );
	void	setAllowedCGI( const std::vector<std::string> & );

	std::vector<std::string>	getContent( void );
	std::string					getPath( void );

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

	std::vector<std::string>		_allowedMethods;	//allowed_method *
	std::pair<int, std::string>		_redirection;		//return *
	std::string						_root;				//root [path]
	bool							_listing;			//autoindex [on/off]
	std::vector<std::string>		_index;				//index *
	std::vector<std::string>		_allowedCGI;		//allowed_CGI *
};


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

	std::vector<int>					extractPort ( void );
	std::vector<std::string>			extractHost( void );
	std::string							extractName( void );
	std::map<std::string, std::string>	extractErrorPages( void );
	long								extractMaxBodySize( void );
	std::map<std::string, Location>		extractLocations( void );

	/* Accessors */
	void	setContent( std::vector<std::string> & );
	void	setPort( const std::vector<int> & );
	void	setHost( const std::vector<std::string> & );
	void	setName( const std::string & );
	void	setErrorPages( const std::map<std::string, std::string> & );
	void	setMaxBodySize( const long & );
	void	setLocations( const std::map<std::string, Location> & );

	std::vector<std::string>			getContent( void );
	std::vector<int>					getPort( void );
	std::vector<std::string>			getHost( void );
	std::string							getName( void );
	std::map<std::string, std::string>	getErrorPages( void );
	size_t								getMaxBodySize( void );
	std::map<std::string, Location>		getLocations( void );
	std::string							getErrorPages(std::string key);

	private :

	/* Attributes */
	std::vector<std::string>	_content;
	bool						_error;

	std::vector<int>					_port;			//listen
	std::vector<std::string>			_host;			//listen

	std::string							_name;			//server_name
														//default host:port
	std::map<std::string, std::string>	_errorPages;	//error_page
	size_t 								_maxBodySize;	//client_max_body_size

	/*=====================================*/

	std::map<std::string, Location>	_locations;

	/*=====================================*/

	/* CGI Part */
};

#endif