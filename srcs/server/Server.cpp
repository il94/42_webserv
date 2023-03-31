/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:57:03 by halvarez          #+#    #+#             */
/*   Updated: 2023/03/31 19:50:14 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <fcntl.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include "Server.hpp"
#define PORT 8080

// Constructors ============================================================= //
Server::Server(void)
{
	static t_sockaddr_in	address;

	address.sin_family		= AF_INET;
	address.sin_addr.s_addr	= INADDR_ANY;
	address.sin_port		= htons( PORT );
	for (size_t i = 0; i < sizeof( address.sin_zero ); i++)
	{
		address.sin_zero[i] = '\0';
	}

	this->_address = reinterpret_cast<t_sockaddr *>(&address);
	//this->_address = (t_sockaddr *)(&address);
	return;
}

Server::Server(const Server & srv __attribute__((unused)) )
{
	int	i = 0;

	this->_srvFd				= srv.getSrvFd();
	this->_address->sa_family	= ( srv.getSockAddr() )->sa_family;
	while (i < 14)
	{
		this->_address->sa_data[i] = ( srv.getSockAddr() )->sa_data[i];
		i++;
	}
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
	return ( *this );
}

// Public member functions ================================================== //
void	Server::run(void)
{
	int			server_fd __attribute__((unused));
	int			new_socket;
	long		valread __attribute__((unused));
	int			addrlen;
	std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	int hfd = open("./html/index.html", O_RDONLY );
	char b[30000];

	read( hfd, b, 30000);
	std::string html(b);

	std::ostringstream oss;
    oss << html.size();
	hello = hello + oss.str() + "\n" + b;
	
	std::cout << hello  << std::endl;
	
	if ( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
	{
		std::cerr << "Error: socket creation failed." << std::endl;
		exit( 1 );
	}

	if (bind( server_fd, this->getSockAddr(), sizeof( t_sockaddr )) < 0 )
	{
		perror("Error: during binding");
		exit( 1 );
	}
	//this->mkSrvSocket();

	if ( listen(server_fd, 10) < 0 )
	{
		std::cerr << "Error: listening" << std::endl;
		exit( 1 );
	}
	while ( 1 )
	{
		std::cout << "========== waiting for connection ==========" << std::endl;
		if ( (new_socket = accept(server_fd, this->getSockAddr(), (socklen_t*)&addrlen)) < 0 )
		{
			std::cerr << "Error: accepting connection" <<std::endl;
			exit( 1 );
		}
		char buffer[30000] = {0};
		read( new_socket, buffer, 30000);
		std::cout << buffer << std::endl;
		write( new_socket, hello.c_str(), hello.size() );
		close( new_socket );
	}
	return;
}

void	Server::mkSrvSocket(void)
{
	this->setSrvFd( socket( AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0 ) );

	if ( bind( this->getSrvFd(), this->getSockAddr(), sizeof( t_sockaddr ) ) == -1 )
	{
		std::cerr << "Error: during binding" << std::endl;
		exit( 1 );
	}
	return;
}

void	Server::setSrvFd(const int fd)
{
	this->_srvFd = fd;
	if ( this->_srvFd == -1 )
	{
		std::cerr << "Error: socket creation failed" << std::endl;
		exit( 1 );
	}
	return;
}

const int &	Server::getSrvFd(void) const
{
	return ( this->_srvFd );
}

Server::t_sockaddr *	Server::getSockAddr(void) const
{
	return ( this->_address );
}

// Private member functions ================================================= //
