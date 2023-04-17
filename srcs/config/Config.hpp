#ifndef CONFIG
#define CONFIG

#include <iostream>

#include <string>
#include <map>
#include <vector>

#include <algorithm>

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
	
	std::string					findInFileContent( const std::string & );
	std::vector<std::string>	multipleFindInFileContent( const std::string & );
	void						printConfig( void );



	short								extractPort ( void );
	std::string							extractHost( void );
	std::string							extractName( void );
	std::map<std::string, std::string>	extractErrorPages( void );
	long								extractMaxBodySize( void );

	std::vector<std::string>	extractAllowedMethods( void );

	std::string	extractRoot( void );
	std::string	extractIndex( void );
	bool		extractListing( void );



	/* Accessors */
	void	setFileContent( std::vector<std::string> & );

	void	setPort( const short & );
	void	setHost( const std::string & );
	void	setName( const std::string & );
	void	setErrorPages( const std::map<std::string, std::string> & );
	void	setMaxBodySize( const long & );

	void	setAllowedMethods( const std::vector<std::string> & );

	void	setRoot( const std::string & );
	void	setIndex( const std::string & );
	void	setListing( const bool & );




	short		getPort( void );
	std::string	getHost( void );
	std::string	getName( void );
	std::map<std::string, std::string>	getErrorPages( void );
	long		getMaxBodySize( void );

	std::vector<std::string>	getAllowedMethods( void );

	std::string	getRoot( void );
	std::string	getIndex( void );
	bool		getListing( void );



	private :

	/* Attributes */
	std::vector<std::string>			_fileContent;
	bool								_error;

	short		_port; //listen [IP + Port = 000.0.0.0:0000]
	std::string	_host; //listen [IP + Port = 000.0.0.0:0000]

	std::string	_name; //server_name [name]
	// default host:port
	std::map<std::string, std::string>	_errorPages; //error_page [path_file.html]
	long 		_maxBodySize; //client_max_body_size [value + unit = 00M]

	/*=====================================*/

	std::vector<std::string>	_allowedMethods; // allowed_method [method name]
	// HTTP redirection
	std::string	_root; //root [path]
	std::string	_index; //index [path]
	bool		_listing; //autoindex [on/off]
	std::string	_directoryRequest;	//error_directory_page [path_file.html]
	// std::vector<std::string>	_contentPath; //location...
	/**/

	/*=====================================*/

	/* CGI Part */
};

#endif