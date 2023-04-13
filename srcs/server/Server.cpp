/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:57:03 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/13 22:16:03 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <fcntl.h>
#include <sstream>
#include <string>

#include "Server.hpp"
#define PORT 8080

// Constructors ============================================================= //
Server::Server(void)
{
	return;
}

Server::Server( const std::string &src )
{
	if (src == "default")
	{
		/* générer un serveur par défaut */
	}
	return;
}


Server::Server(const Server & srv __attribute__((unused)) )
{
	*this = srv;
	return;
}

// Destructor =============================================================== ///
Server::~Server(void)
{
	return;
}


// Operators ================================================================ ///
Server &	Server::operator=(const Server & srv __attribute__((unused)) )
{
	config = srv.config;
	return ( *this );
}

// Member functions ========================================================= ///

void		Server::setConfig(std::vector< std::string > & srv)
{
	this->config.setFileContent(srv);
	
	/*==================================*/
	
	config.setPort(config.extractPort());
	config.setHost(config.extractHost());
	config.setSocket(config.extractSocket());
	config.setName(config.extractName());
	config.setErrorPages(config.extractErrorPages());
	config.setMaxBodySize(config.extractMaxBodySize());
	
	

	/*==================================*/

	config.setAllowedMethods(config.extractAllowedMethods());
	
	config.setRoot(config.extractRoot());
	config.setIndex(config.extractIndex());
	
	/*==================================*/
	
	config.printConfig();
	
	/*==================================*/
	
}





// void	Server::run(void)
// {
// 	int			server_fd;
// 	int			new_socket;
// 	long		valread __attribute__((unused));
// 	int			addrlen;
// 	std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
// 	int hfd = open("./html/index.html", O_RDONLY );
// 	char b[30000];

// 	read( hfd, b, 30000);
// 	std::string html(b);

// 	std::ostringstream oss;
//     oss << html.size();
// 	hello = hello + oss.str() + "\n" + b;
	
// 	std::cout << hello  << std::endl;
// 	struct sockaddr_in address;
	
// 	if ( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
// 	{
// 		std::cerr << "Error: socket creation failed." << std::endl;
// 		exit( 1 );
// 	}

// 	address.sin_family			= AF_INET;
// 	address.sin_addr.s_addr	= INADDR_ANY;
// 	address.sin_port			= htons( PORT );

// 	for (size_t i = 0; i < sizeof( address.sin_zero ); i++) {
// 		address.sin_zero[i] = '\0';
// 	}

// 	if (bind( server_fd, (struct sockaddr *)&address, sizeof(address)) < 0 )
// 	{
// 		perror("Error: during binding");
// 		exit( 1 );
// 	}

// 	if ( listen(server_fd, 10) < 0 )
// 	{
// 		std::cerr << "Error: listening" << std::endl;
// 		exit( 1 );
// 	}
// 	while ( 1 )
// 	{
// 		std::cout << "========== waiting for connection ==========" << std::endl;
// 		if ( (new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0 )
// 		{
// 			std::cerr << "Error: accepting connection" <<std::endl;
// 			exit( 1 );
// 		}
// 		char buffer[30000] = {0};
// 		read( new_socket, buffer, 30000);
// 		std::cout << buffer << std::endl;
// 		write( new_socket, hello.c_str(), hello.size() );
// 		close( new_socket );
// 	}
// 	return;
// }

