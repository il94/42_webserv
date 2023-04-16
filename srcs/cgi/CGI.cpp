/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:09:53 by auzun             #+#    #+#             */
/*   Updated: 2023/04/16 03:26:17 by auzun            ###   ########.fr       */
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
	_querystring = (char *)_request.getRequestContent().c_str();
}


std::string	CGI::execCGI(std::string scriptPath)
{
	int			pid;
	int			pipefd[2];
	std::string	sendedBody = "";

	setArgs(scriptPath);
	setEnv();
	setQueryString();

	if (pipe(pipefd) < 0)
	{
		std::cerr << "Pipe failed" << std::endl;
		return ("");
	}
	pid = fork();
	if (pid < 0)
	{
		std::cerr << "Fork failed" << std::endl;
		return ("");
	}
	else if (pid == 0)
	{
		char	* const * nll = NULL;
		if (dup2(pipefd[0], 0) < 0)
		{
			std::cerr << "Dup2 failed" << std::endl;
			exit(1); // exit fatal..
		}
		if (dup2(pipefd[1], 1) < 0)
		{
			std::cerr << "Dup2 failed" << std::endl;
			exit(1); // exit fatal...
		}
		char	**args = &_args[0];
		execve(args[0], args, _env);
		close(pipefd[1]);
		close(pipefd[0]);
		std::cerr << "Execve failed" << std::endl;
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};
		if (pipefd[1] > -1)
		{
			write(pipefd[1], _querystring, strlen(_querystring));
			close(pipefd[1]);
		}
		waitpid(-1, NULL, 0);
		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(pipefd[0], buffer, CGI_BUFSIZE - 1);
			sendedBody += buffer;
		}
	}
	if (pipefd[0] > -1)
			close(pipefd[0]);
	return (sendedBody);
}