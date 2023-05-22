#include "CGI.hpp"

CGI::CGI(){}

CGI::CGI(Request &request): _request(request) {}

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
	if (header["REQUEST_METHOD"] == "GET")
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

std::string CGI::execCGI(std::string scriptPath)
{
		pid_t			pid;
		int				pipefd_input[2];
		int				pipefd_output[2];

		std::string		output = "";
		const char		*failedSTR = "Status: 500\r\n\r\n";

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
			write(pipefd_output[1], failedSTR, strlen(failedSTR));
			close(pipefd_input[0]);
			close(pipefd_output[1]);
			for (size_t i = 0; _env[i]; i++)
				delete[] _env[i];
			delete[] _env;
			exit(EXIT_FAILURE);
		}
		else if (pid > 0)
		{
			// parent process
			close(pipefd_input[0]);
			write(pipefd_input[1], _request.getRequestContent().c_str(), _request.getRequestContent().size());
			close(pipefd_input[1]);

			close(pipefd_output[1]);
			char buffer[1024] = {0};
			while (read(pipefd_output[0], buffer, 1023) > 0) {
				output += buffer;
			}
			close(pipefd_output[0]);

			wait(NULL);
		}
		else
		{
			std::cerr << RED << "Fork failed\n" << END << std::endl;
			return std::string(failedSTR);
		}
		for (size_t i = 0; _env[i]; i++)
			delete[] _env[i];
		delete[] _env;
		return output;
}