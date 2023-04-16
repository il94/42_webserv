/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:09:53 by auzun             #+#    #+#             */
/*   Updated: 2023/04/16 23:30:43 by auzun            ###   ########.fr       */
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
		//std::cout << _env[j] << std::endl ;
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
    //const char *args[] = {"/bin/bash/html/cgi_test/cgi-bin/contact.py", NULL};
    const char *env[] = {"REQUEST_METHOD=POST", "CONTENT_TYPE=application/x-www-form-urlencoded", "CONTENT_LENGTH=43", NULL};
    const char *data = "message=Hello&nom=John&email=johnxample.com";
    int data_len = strlen(data);
	std::cout << data_len << data[35] << std::endl;
	 write(2, data, data_len);
	 std::cout << std::endl;
    int pipefd_input[2];
    int pipefd_output[2];
    pipe(pipefd_input);
    pipe(pipefd_output);

    pid_t pid = fork();
    if (pid == 0) {
        // child process
        dup2(pipefd_input[0], STDIN_FILENO);
        close(pipefd_input[1]);

        dup2(pipefd_output[1], STDOUT_FILENO);
        close(pipefd_output[0]);

        char * const *nll = NULL;
        execve(scriptPath.c_str(), nll, (char* const*)env);
		std::cerr << "wsh papa allo " << std::endl;
    } else if (pid > 0) {
        // parent process
        close(pipefd_input[0]);
        write(pipefd_input[1], data, data_len);
        close(pipefd_input[1]);

        close(pipefd_output[1]);
        char buffer[1024];
        std::string output;
        while (read(pipefd_output[0], buffer, 1024) > 0) {
            output += buffer;
        }
		std::cerr<<output << "letext" << std::endl;
        close(pipefd_output[0]);

        wait(NULL);

        return output;
    } else {
        // fork failed
        std::cerr << "fork failed\n";
        return "1";
    }
	  return "1";
	// int			pid;
	// const char * test =  "nom=dsf&email=dsf%40sdf.com&message=sdf";
	// int			pipefd[2];
	// std::string	sendedBody = "";
	
	// setEnv();
	// setQueryString();

	// if (pipe(pipefd) < 0)
	// {
	// 	std::cerr << "Pipe failed" << std::endl;
	// 	return ("");
	// }

	// //write(pipefd[0], test, strlen(test));
	// pid = fork();
	// if (pid < 0)
	// {
	// 	std::cerr << "Fork failed" << std::endl;
	// 	return ("");
	// }
	// else if (pid == 0)
	// {
	// 	char * const *nll = NULL;
	// 	if (dup2(pipefd[0], 0) < 0)
	// 	{
	// 		std::cerr << "Dup2 failed" << std::endl;
	// 		exit(1); // exit fatal..
	// 	}
	// 	if (dup2(pipefd[1], 1) < 0)
	// 	{
	// 		std::cerr << "Dup2 failed" << std::endl;
	// 		exit(1); // exit fatal...
	// 	}
		
	// 	execve(scriptPath.c_str(), nll, _env);
	// 	close(pipefd[1]);
	// 	close(pipefd[0]);
	// 	std::cerr << "Execve failed" << std::endl;
	// }
	// else
	// {
	// 	//char	buffer[CGI_BUFSIZE] = {0};
	// 	if (pipefd[1] > -1)
	// 	{
	// 		//write(pipefd[1], _request.getRequestBody().substr(1, _request.getRequestBody().size()).c_str(), _request.getRequestBody().size());
	// 		write(pipefd[1], test, strlen(test));
	// 		close(pipefd[1]);
	// 	}
	// 	waitpid(-1, NULL, 0);
	// 	//int ret = 1;
	// 	// while (ret > 0)
	// 	// {
	// 	// 	memset(buffer, 0, CGI_BUFSIZE);
	// 	// 	ret = read(pipefd[0], buffer, CGI_BUFSIZE - 1);
	// 	// 	sendedBody += buffer;
	// 	// 	std::cerr << std::endl << "string = " <<  sendedBody << std::endl;

	// 	// }
	// }
	// if (pipefd[0] > -1)
	// 		close(pipefd[0]);
	// std::cerr << "finito" << std::endl;
	// return (sendedBody);
}