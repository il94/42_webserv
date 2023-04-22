/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:57:03 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/22 16:22:51 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netdb.h>

#include "Server.hpp"
#include "../http/request/Request.hpp"
#include "../http/response/Response.hpp"

#define PORT		8080
#define MAX_EVENTS	5

// Constructors ============================================================= //
Server::Server(void) : _name( "webserv" ), _port( PORT ), _srvfd( -1 ), _eplfd( -1 ), _address( NULL ), _eplev( NULL ), _flag( EMPTY )
{
	this->_setSockAddr();
	this->_mkSrvSocket();
	this->_setEpollEvent();
	this->_mkEpoll();
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

Server::Server(const Server & srv) : _name( srv._getName() ), _srvfd( dup( srv._getFd( SRV ) ) ), _eplfd( dup( srv._getFd( EPL ) ) ), _address( NULL ), _eplev( NULL ), _flag ( EMPTY )
{
	int	i = 0;

	if (this->_address == NULL)
		this->_setSockAddr();
	this->_srvfd				= dup( srv._getFd( SRV ) );
	this->_eplfd				= dup( srv._getFd( EPL ) );
	this->_address->sa_family	= ( srv._getSockAddr() )->sa_family;
	while (i < 14)
	{
		this->_address->sa_data[i] = ( srv._getSockAddr() )->sa_data[i];
		i++;
	}
	if (this->_eplev == NULL)
		this->_setEpollEvent();
	this->_eplev->events = srv._eplev->events;
	this->_eplev->data.fd = srv._getFd( SRV );
	return;
}

// Destructor =============================================================== //
Server::~Server(void)
{
	if ( this->_srvfd != -1 && close( this->_srvfd ) == -1 )
		this->_srvError(__func__, __LINE__, "close srvfd");
	if ( this->_eplfd != -1 && close( this->_eplfd ) == -1 )
		this->_srvError(__func__, __LINE__, "close eplfd");
	return;
}

// Operators ================================================================ //
Server &	Server::operator=(const Server & srv)
{
	_config = srv._config;
	return ( *this );
}

// Member functions ========================================================= ///

void		Server::setConfig(std::vector< std::string > & srv)
{
	this->_config.setContent(srv);
	
	/*==================================*/
	
	_config.setPort(_config.extractPort());
	_config.setHost(_config.extractHost());
	_config.setName(_config.extractName());
	_config.setErrorPages(_config.extractErrorPages());
	_config.setMaxBodySize(_config.extractMaxBodySize());
	

	/*==================================*/

	_config.setLocations(_config.extractLocations());
	
	/*==================================*/
	
	_config.display();
	
	/*==================================*/
	
}

// Public member functions ================================================== //
void	Server::run(void)
{
	int				cliSocket	__attribute__((unused)) = -1;
	int				nbEvents	__attribute__((unused)) = -1;
	int				addrlen		__attribute__((unused));
	t_epoll_event	cliEvents[ MAX_EVENTS ] __attribute__((unused));

	// Testing data = will be removed ======================================= //
	std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	int hfd = open("./html/index.html", O_RDONLY );
	char b[30000];

	read( hfd, b, 30000);
	std::string html(b);

	std::ostringstream oss;
    oss << html.size();
	hello = hello + oss.str() + "\n" + b;
	
	//std::cout << hello  << std::endl;
	// ====================================================================== //

	// Connection management ================================================ //
	if ( listen( this->_getFd( SRV ), 100 ) == -1 )
		this->_srvError(__func__, __LINE__, "listen");
	this->_log("listening");
	while ( this->_flag != ERROR )
	{
		nbEvents = epoll_wait( this->_getFd( EPL ), cliEvents, MAX_EVENTS, -1);
		for (int i = 0; i < nbEvents; ++i)
		{
			if ( cliEvents[i].data.fd == this->_getFd( SRV ) )
			{
				cliSocket = accept( this->_getFd( SRV ), this->_address, reinterpret_cast<socklen_t *>(&addrlen) );
				if ( cliSocket == -1 )
					this->_srvError(__func__, __LINE__, "accept");
				else
					this->_log("connection established");
				if ( cliEvents[i].events & EPOLLIN & ~EPOLLHUP )
				{
					this->_log("sending data to client");
					send( cliSocket, hello.c_str(), hello.size(), 0 );
				}
				else if ( cliEvents[i].events & EPOLLOUT & ~EPOLLHUP )
				{
					this->_log("receiving request from client");
				}
				else
				{
					this->_log("I don't know what happens");
				}
			}
			else
				this->_log("connection refused");
			close( cliSocket );
			this->_log("listening");
		}
	}
	// ====================================================================== //
	return;
}

// Private member functions ================================================= //
void	Server::_setSockAddr(void)
{
	static t_sockaddr_in	address;

	address.sin_family		= AF_INET;
	address.sin_addr.s_addr	= INADDR_ANY;
	address.sin_port		= htons( this->_getPort() );
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

	fd = socket( AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0 ); 
	if (fd == -1)
		this->_srvError(__func__, __LINE__, "socket");
	if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) ) == -1 )
		this->_srvError(__func__, __LINE__, "setsockopt");
	if ( bind( fd, this->_getSockAddr(), sizeof( t_sockaddr ) ) == -1 )
		this->_srvError(__func__, __LINE__, "bind");

	this->_srvfd = fd;
	return;
}

void	Server::_setEpollEvent(void)
{
	static t_epoll_event	eplev;

	eplev.events = EPOLLIN | EPOLLOUT | EPOLLHUP;
	eplev.data.fd = this->_getFd( SRV );
	this->_eplev = &eplev;
	return;
}

void	Server::_mkEpoll(void)
{
	int	fd	= -1;

	fd = epoll_create( 1 );
	if ( fd == -1 )
		this->_srvError(__func__, __LINE__, "epoll_create");
	this->_eplfd = fd;
	if ( epoll_ctl( this->_getFd( EPL ), EPOLL_CTL_ADD, this->_getFd( SRV ), this->_eplev ) == -1)
		this->_srvError(__func__, __LINE__, "epoll_ctl");
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
		default :
			throw;
	}
}

Server::t_sockaddr *	Server::_getSockAddr(void) const
{
	return ( this->_address );
}

Server::t_epoll_event *	Server::_getEpollEvent(void) const
{
	return ( this->_eplev );
}

void	Server::_srvError(const char *func, const int line, const char *msg)
{
	std::cerr << func << ":" << line - 1 << ":";
	this->_flag = ERROR;
	perror( msg );
	//exit( 1 );
	return;
}

void	Server::_log(const char *msg) const
{
	std::cout << "\t" << this->_getName() << "[" << this->_getPort() << "] : ";
	std::cout << msg << std::endl;
	return;
}

void	Server::_setPort(const int port)
{
	this->_port = port;
	return;
}

const int &	Server::_getPort(void) const
{
	return ( this->_port );
}

void	Server::_setName(const std::string name)
{
	this->_name = name;
	return;
}

const std::string &	Server::_getName(void) const
{
	return ( this->_name );
}
