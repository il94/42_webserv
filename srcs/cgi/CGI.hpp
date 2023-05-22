#ifndef CGI_HPP
# define CGI_HPP

# include "../http/request/Request.hpp"
# include <vector>
# include <algorithm>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <stdlib.h>

# define	CGI_BUFSIZE 42424

class CGI
{
	public:
		CGI();
		CGI(Request &request);
		~CGI();
		
		
		void	setUploadInfo(std::string uploadFilename, std::string uploadPath);
		void	setEnv();
		std::string	execCGI(std::string scriptName);
		std::vector<std::pair<std::string,std::string> >	extractCookieFileContent();
	private:
		Request			_request;
		std::string		_uploadFilename;
		std::string		_uploadPath;

		char				**_env;
};

# endif