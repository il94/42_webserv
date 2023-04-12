/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:09:53 by auzun             #+#    #+#             */
/*   Updated: 2023/04/12 21:57:54 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI()
{
}

CGI::CGI(Request &request): _body(request.getRequestBody())
{
}

CGI::~CGI(){}

std::string	CGI::execCGI(std::string scriptName)
{
	int			pid;
	int			pipefd[2];
	std::string	sendedBody = "";

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
		execve(scriptName.c_str(), nll, NULL);
		close(pipefd[1]);
		close(pipefd[0]);
		std::cerr << "Execve failed" << std::endl;
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};
		if (pipefd[1] > -1)
			close(pipefd[1]);
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