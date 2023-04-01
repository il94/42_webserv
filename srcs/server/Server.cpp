/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:57:03 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/01 18:05:29 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdio>
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
	return;
}

Server::Server(const Server & srv __attribute__((unused)) )
{
	int	i = 0;

	this->_srvfd				= srv.getSrvFd();
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
	int			new_socket;
	int			addrlen;

	// Testing data ========================================================= //
	std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	int hfd = open("./html/index.html", O_RDONLY );
	char b[30000];

	read( hfd, b, 30000);
	std::string html(b);

	std::ostringstream oss;
    oss << html.size();
	hello = hello + oss.str() + "\n" + b;
	
	std::cout << hello  << std::endl;
	// ====================================================================== //
	
	this->mkSrvSocket();

	// Conncetion management ================================================ //
	if ( listen( this->getSrvFd(), 10) < 0 )
	{
		perror("Error");
		exit( 1 );
	}
	while ( 1 )
	{
		std::cout << "========== waiting for connection ==========" << std::endl;
		if ( (new_socket = accept( this->getSrvFd(), this->getSockAddr(), (socklen_t*)&addrlen)) < 0 )
		{
			perror("Error");
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
	int	fd;
	int	opt = 1;

	fd = socket( AF_INET, SOCK_STREAM /*| SOCK_NONBLOCK*/, 0 ); 
	if (fd == -1)
	{
		perror("Error");
		exit( 1 );
	}
	if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) ) == -1 )
	{
		perror("Error");
		exit( 1 );
	}
	if ( bind( fd, this->getSockAddr(), sizeof( t_sockaddr ) ) == -1 )
	{
		perror("Error");
		exit( 1 );
	}
	this->setSrvFd( fd );
	return;
}

void	Server::setSrvFd(const int & fd)
{
	this->_srvfd = fd;
	if ( this->_srvfd == -1 )
	{
		std::cerr << "Error: invalid fd value : -1" << std::endl;
		exit( 1 );
	}
	return;
}

const int & 	Server::getSrvFd(void) const
{
	return ( this->_srvfd );
}

Server::t_sockaddr *	Server::getSockAddr(void) const
{
	return ( this->_address );
}

// Private member functions ================================================= //
