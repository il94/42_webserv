#ifndef	RESPONSE_HPP
# define RESPONSE_HPP

#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
# include "../../../include/webserv.hpp"
#include "../../utils/utils.hpp"
#include "../request/Request.hpp"
#include "../../config/Config.hpp"
#include "../../cgi/CGI.hpp"
//#include "../../server/Client.hpp"

class Response
{
	public :
		// Constructors/Destructor
		Response(void);
		Response(Request &request, const Config &config, int port, std::string host);
		Response(const Response &src);
		~Response(void);

		// Assignment operator
		Response &	operator=(const Response &src);

		// Methods
		void	generate();
		void	GET(void);
		void	POST(void);
		void	DELETE(void);

		// Configuration methods
		void	setRequest(Request &request);
		void	setConfig(Config &config);
		void	setContent(std::vector<unsigned char> *vec);
		void	setPath();
		void	setContentType(std::string path);
		void	setCode(int code);
		void	setContentLength(size_t size);

		// Data access methods
		std::string	getStatuMsg();
		int			getUploadStatu();
		std::string	getUploadFileName();
		std::string	getUploadPath();
		std::string	getResponse();

		// Response utility methods
		int			readContent();
		std::string	readErrorPage(const std::string &path);
		int			fileExist(std::string path);
		int			isFile(std::string path);
		int			isDir(std::string path);
    std::vector<std::pair<std::string,std::string> >	splitCookieHeader(const std::string &src);

		bool		findCGI();
		bool		findCookie();
		void		extractCookie();
		Location	findLocation();

		// Upload management methods
		std::string	extractMPFD_Header();
		void		uploadSuccess();
		void		uploadFailed();
		void		upload();

		// Index generation methods
		std::string	generateAutoIndex();

		// Header management methods
		std::string	generateHeader(size_t size, std::string path);
		std::string	writeHeader();
		void		initStatusMsg();

	private :

		int			_port;
		std::string	_host;

		std::string	_path;

		/*Upload*/
		int							_uploadStatu;
		std::string					_boundary;
		std::string					_uploadPath;
		std::string					_uploadFileName;
		std::string					_controler;
		std::vector<std::string>	_files;
		std::vector<std::string>	_headerName;
		std::vector<unsigned char>	* _content;

		/*Header*/
		std::string					_contentLength;
		std::string					_contentType;
		std::map<int, std::string>	_statusMsg;
		int			_code;
		std::vector<std::string>	_cookies;
		/*------*/
		
		Request		_request;
		Config		_config;
		std::string	_response;
		Location	_location;
};


#endif