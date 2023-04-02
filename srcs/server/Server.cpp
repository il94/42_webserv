/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:57:03 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/02 18:29:01 by halvarez         ###   ########.fr       */
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
#include <sys/epoll.h>

#include "Server.hpp"
#define PORT 8080

// Constructors ============================================================= //
Server::Server(void) : _srvfd( -1 ), _eplfd( -1 )
{
	this->_setSockAddr();
	this->_mkSrvSocket();
	this->_mkEpoll();
	return;
}

Server::Server(const Server & srv)
{
	int	i = 0;

	this->_srvfd				= srv._getFd( SRV );
	this->_eplfd				= srv._getFd( EPL );
	this->_address->sa_family	= ( srv._getSockAddr() )->sa_family;
	while (i < 14)
	{
		this->_address->sa_data[i] = ( srv._getSockAddr() )->sa_data[i];
		i++;
	}
	//-----------> don't forget to cpy epoll_event <-----------
	return;
}

// Destructor =============================================================== //
Server::~Server(void)
{
	if ( this->_srvfd != -1 && close( this->_srvfd ) == -1 )
		perror("Error closing server fd");
	if ( this->_eplfd != -1 && close( this->_eplfd ) == -1 )
		perror("Error closing epoll fd");
	return;
}

// Operators ================================================================ //
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
	

	// Conncetion management ================================================ //
	if ( listen( this->_getFd( SRV ), 10) < 0 )
	{
		perror("Error");
		exit( 1 );
	}
	while ( 1 )
	{
		std::cout << "========== waiting for connection ==========" << std::endl;
		if ( (new_socket = accept( this->_getFd( SRV ), this->_getSockAddr(), (socklen_t*)&addrlen)) < 0 )
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

// Private member functions ================================================= //
void	Server::_setSockAddr(void)
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

void	Server::_mkSrvSocket(void)
{
	int	fd	= -1;
	int	opt = 1;

	fd = socket( AF_INET, SOCK_STREAM /*| SOCK_NONBLOCK*/, 0 ); 
	if (fd == -1)
	{
		perror("Socket error");
		exit( 1 );
	}
	if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) ) == -1 )
	{
		perror("Error setting socket options");
		exit( 1 );
	}
	if ( bind( fd, this->_getSockAddr(), sizeof( t_sockaddr ) ) == -1 )
	{
		perror("Bind error");
		exit( 1 );
	}
	this->_srvfd = fd;
	return;
}

void	Server::_mkEpoll(void)
{
	static t_epoll_event	eplev;
	int						fd	= -1;

	fd = epoll_create( 1 );
	if ( fd == -1 )
	{
		perror("Error creating epoll instance");
		exit( 1 );
	}
	this->_eplfd = fd;
	return;
}

const int & 	Server::_getFd( const t_fd FD ) const
{
	switch ( FD )
	{
		case SRV :
			return ( this->_srvfd );
		case EPL :
			return ( this->_eplfd );
	}
}

Server::t_sockaddr *	Server::_getSockAddr(void) const
{
	return ( this->_address );
}

/*
	//void				setFd(const t_fd FD, const int & fd);
	void	Server::setFd(const t_fd FD, const int & fd)
	{
		switch ( FD )
		{
			case SRV :
				this->_srvfd = fd;
				return;
			case EPL :
				this->_eplfd = fd;
				return;
			default :
				std::cerr << "Error: invalid fd value : " << fd << std::endl;
				exit( 1 );
				return;
		}
		return;
	}
*/
