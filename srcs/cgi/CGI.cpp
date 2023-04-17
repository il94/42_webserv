/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:09:53 by auzun             #+#    #+#             */
/*   Updated: 2023/04/18 00:33:37 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI()
{
}

CGI::CGI(Request &request): _request(request), _body(request.getRequestBody())
{
}

CGI::~CGI(){}

void	CGI::setArgs(std::string path)
{
	_args.push_back((char *)"/bin/bash");
	_args.push_back((char *)path.c_str());
	_args.push_back(NULL);
}

void	CGI::setEnv()
{
	std::map<std::string, std::string>	header = _request.getHeaderM();
	header["REQUEST_METHOD"] = _request.getMethod();
	header["SERVER_PROTOCOL"] = "HTTP/1.1";
	
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

void	CGI::setQueryString()
{
	_querystring = new char[_request.getRequestContent().size() + 1];
	_querystring = strcpy(_querystring, (const char *)_request.getRequestContent().c_str());
}


std::string CGI::execCGI(std::string scriptPath)
{
		int pipefd_input[2];
		int pipefd_output[2];

		pipe(pipefd_input);
		pipe(pipefd_output);

		setEnv();
		pid_t pid = fork();
		
		if (pid == 0) {
			// child process
			dup2(pipefd_input[0], STDIN_FILENO);
			close(pipefd_input[1]);

			dup2(pipefd_output[1], STDOUT_FILENO);
			close(pipefd_output[0]);

			char * const *nll = NULL;
			execve(scriptPath.c_str(), nll, _env);
		} else if (pid > 0) {
			// parent process
			close(pipefd_input[0]);
			write(pipefd_input[1], _request.getRequestContent().c_str(), _request.getRequestContent().size());
			close(pipefd_input[1]);

			close(pipefd_output[1]);
			char buffer[1024];
			std::string output;
			while (read(pipefd_output[0], buffer, 1024) > 0) {
				output += buffer;
			}
			close(pipefd_output[0]);

			wait(NULL);

			return output;
		} else {
			// fork failed
			std::cerr << "fork failed\n";
			return "";
		}
		return "";
}