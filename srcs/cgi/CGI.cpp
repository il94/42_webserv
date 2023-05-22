#include "CGI.hpp"

CGI::CGI(){}

CGI::CGI(Request &request): _request(request), _uploadFilename(""), _uploadPath("")
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

CGI::~CGI(){}

void	CGI::setUploadInfo(std::string uploadFilename, std::string uploadPath)
{
	_uploadFilename = uploadFilename;
	_uploadPath = uploadPath;
}

void	CGI::setEnv()
{
	std::map<std::string, std::string>	header = _request.getHeaderM();
	header["REQUEST_METHOD"] = _request.getMethod();
	header["SERVER_PROTOCOL"] = "HTTP/1.1";
	if (_request.getMethod() == "GET")
		header["QUERY_STRING"] = _request.getRequestContent();
	header["FILE_NAME"] = _uploadFilename;
	header["UPLOAD_PATH"] = _uploadPath;


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
