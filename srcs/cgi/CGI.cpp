#include "CGI.hpp"

CGI::CGI(){}

CGI::CGI(Request &request): _request(request)
{
	std::vector<std::pair<std::string,std::string> >	cookies = extractCookieFileContent();

	if (cookies.empty() == false)
	{
		std::string	requestWithCookies;
		
		if (_request.getRequestContent().empty() == false)
			requestWithCookies = _request.getRequestContent() + '&';

		for (std::vector<std::pair<std::string,std::string> >::iterator it = cookies.begin();
			it != cookies.end(); it++)
		{
			requestWithCookies += it->first + '=';
			requestWithCookies += it->second;
			if (it + 1 != cookies.end())
				requestWithCookies += '&';
		}
		_request.setRequestContent(requestWithCookies);
	}
}

CGI::CGI(const CGI &src)
{
	_request = src._request;
	_env = src._env;
}

CGI::~CGI(){}

CGI	&	CGI::operator=(const CGI & src)
{
	_request = src._request;
	_env = src._env;
	return (*this);
}

void	CGI::setEnv()
{
	std::map<std::string, std::string>	header = _request.getHeaderM();
	header["REQUEST_METHOD"] = _request.getMethod();
	header["SERVER_PROTOCOL"] = "HTTP/1.1";
	if (_request.getMethod() == "GET")
		header["QUERY_STRING"] = _request.getRequestContent();
	_env = new char *[header.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = header.begin(); i!= header.end(); i++)
	{
		std::string	tmp = i->first + "=" + i->second;
		_env[j] = new char[tmp.size() + 1];
		_env[j] = strcpy(_env[j], (const char *)tmp.c_str());
		j++;
	}
	_env[j] = NULL;
}

extern bool running;
std::string CGI::execCGI(std::string scriptPath)
{
		pid_t			pid;
		int				pipefd_input[2];
		int				pipefd_output[2];

		std::string		output = "";
		const char		*internalError = "Status: 500\r\n\r\n";
		const char		*timeOutError = "Status: 504\r\n\r\n";

		setEnv();
		pipe(pipefd_input);
		pipe(pipefd_output);
		
		pid = fork();
		
		if (pid == 0) {
			// child process
			char * const *nll = NULL;

			dup2(pipefd_input[0], STDIN_FILENO);
			close(pipefd_input[1]);

			dup2(pipefd_output[1], STDOUT_FILENO);
			close(pipefd_output[0]);

			execve(scriptPath.c_str(), nll, _env);
			std::cerr << RED << "Execve failed\n" << END << std::endl;

			write(pipefd_output[1], internalError, strlen(internalError));
			close(pipefd_input[0]);
			close(pipefd_output[1]);
			for (size_t i = 0; _env[i]; i++)
				delete[] _env[i];
			delete[] _env;
			running = false;
			return (output);
		}
		else if (pid > 0)
		{
			// parent process
			close(pipefd_input[0]);

			write(pipefd_input[1], _request.getRequestContent().c_str(), _request.getRequestContent().size());
			close(pipefd_input[1]);

			close(pipefd_output[1]);

			fcntl(pipefd_output[0], F_SETFL, O_NONBLOCK);
			clock_t	start = clock();
			char buffer[1024] = {0};

			while (1) 
			{
				if (read(pipefd_output[0], buffer, 1023) > 0)
					output += buffer;
				if (waitpid(-1, NULL, WNOHANG) == -1) 
					break ;
				else if (static_cast<float>((std::clock() - start) / CLOCKS_PER_SEC)  >= TIMEOUT_CGI)
				{
					output = timeOutError;
					kill(pid, SIGTERM);
					break ;
				}
			}
			close(pipefd_output[0]);

		}
		else
		{
			std::cerr << RED << "Fork failed\n" << END << std::endl;
			return std::string(internalError);
		}
		for (size_t i = 0; _env[i]; i++)
			delete[] _env[i];
		delete[] _env;
		return output;
}

std::vector<std::pair<std::string,std::string> >	CGI::extractCookieFileContent()
{
	std::vector<std::pair<std::string,std::string> >	cookies;
	std::ifstream										file("cookies.txt");

	if (file.is_open())
	{
		std::string								line;
		std::pair<std::string,std::string>		element;

		while (std::getline(file, line))
		{
			element.first = line;
			std::getline(file, line);
			element.second = line;
			std::getline(file, line);
			cookies.push_back(element);
		}
		file.close();
	}
	return (cookies);
}
