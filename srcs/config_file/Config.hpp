#ifndef CONFIG
#define CONFIG

#include <iostream>

#include <string>
#include <map>
#include <vector>

template <typename T>
void	print_test(T str)
{
	std::cout << str << std::endl;
}

class Config : public std::map<std::string, std::string>
{
	public :

	/* Constructors */
	Config();
	Config(const Config&);
	~Config();

	/* Overloads */
	Config&	operator=(const Config&);

	/* Methods */
	
	std::string	findInFileContent( const std::string & );
	std::vector<std::string>	multipleFindInFileContent( const std::string & );
	void		printConfig( void );



	std::string	extractPort ( void );
	std::string	extractHost( void );
	std::string	extractSocket( void );
	std::string	extractName( void );
	std::map<int, std::string>	extractErrorPages( void );
	// std::string	extractMaxBodySize( void );


	/* Accessors */
	void	setFileContent( std::vector<std::string> & );

	void	setPort( const std::string & );
	void	setHost( const std::string & );
	void	setSocket( const std::string & );
	void	setName( const std::string & );
	void	setErrorPages( const std::map<int, std::string> & );
	void	setMaxBodySize( const std::string & );

	std::string	getPort (void );
	std::string	getHost( void );
	std::string	getSocket( void );
	std::string	getName( void );
	std::map<int, std::string>	getErrorPages( void );
	std::string	getMaxBodySize ( void );


	private :

	/* Attributes */
	std::vector<std::string>			_fileContent;
	// std::map<std::string, std::string>	config;

	std::string	_port; //listen [IP + Port = 000.0.0.0:0000]
	std::string	_host; //listen [IP + Port = 000.0.0.0:0000]
		// OU
	std::string	_socket; //listen [IP + Port = 000.0.0.0:0000]

	std::string	_name; //server_name [name]
	// default host:port
	std::map<int, std::string>	_errorPages; //error_page [path_file.html]
	std::string	_maxBodySize; //client_max_body_size [value + unit = 00M]

	/*=====================================*/

	// std::string	_httpMethods[3]; // default GET, POST and DELETE ????
	// HTTP redirection
	// std::string	_rootedPath; //root [path]
	// std::vector<std::string>	_contentPath; //location...
	/**/
	// std::string	_directoryRequest;	//error_directory_page [path_file.html]

	/*=====================================*/

	/* CGI Part */
};

#endif