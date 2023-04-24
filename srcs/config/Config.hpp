#ifndef CONFIG
#define CONFIG

# include "../../include/webserv.hpp"
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