#ifndef CGI_HPP
# define CGI_HPP
# define TIMEOUT_CGI 5

# include "../http/request/Request.hpp"
# include <vector>
# include <algorithm>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <ctime>
# include <fcntl.h>
# include <csignal>



# define	CGI_BUFSIZE 42424

class CGI
{
	public:
		CGI();
		CGI(Request &request);
		CGI(const CGI &src);
		~CGI();

		CGI	&	operator=(const CGI & src);

		void	setEnv();
		std::string	execCGI(std::string scriptName);
		std::vector<std::pair<std::string,std::string> >	extractCookieFileContent();
	private:
		Request			_request;
		char				**_env;
};

# endif