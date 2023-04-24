/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:57:03 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/24 18:50:11 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>

#include "Server.hpp"
#include "../http/request/Request.hpp"
#include "../http/response/Response.hpp"

#define DEF_PORT	8080
#define MAX_EVENTS	64

// Constructors ============================================================= //
Server::Server(void) : _nbSrv( 1 ), /*_flags( 1, EMPTY ),*/ _names( 1, "webserv" ), _ports( 1, DEF_PORT ),
	_sockaddr(  ), _eplevs(  ), _srvfd(  ) 
{
	this->_setSockaddr();
	this->_setEplevs();
	this->_eplfd = epoll_create( 1 );
	if ( this->_eplfd == -1 )
		this->_log(ERROR, -1, __func__, __LINE__, "epoll_create");
	return;
}

Server::Server( const Server & srv ) : _nbSrv( srv._nbSrv ), _names( srv._names ),
	_ports( srv._ports ), _sockaddr( srv._sockaddr ), _eplevs( srv._eplevs ), _srvfd(  )
{
	size_t	i	= 0;

	while ( i < this->_getNbSrv() )
	{
		this->_srvfd.push_back( dup( srv._getSrvFd( i ) ) );
		this->_eplevs[i].data.fd = this->_getSrvFd( i );
		i++;
	}
	this->_eplfd = epoll_create( 1 );
	if ( this->_eplfd == -1 )
		this->_log(ERROR, -1, __func__, __LINE__, "epoll_create");
	return;
}

// Destructor =============================================================== //
Server::~Server(void)
{
	size_t	i = 0;

	while ( i < this->_getNbSrv() )
	{
		close( this->_getSrvFd( i ) );
		close( this->_getEplFd() );
		i++;
	}
	return;
}

// Operators ================================================================ //
Server &	Server::operator=(const Server & srv __attribute__((unused)))
{
	return ( *this );
}

// Member functions ========================================================= ///

void		Server::setConfig(std::vector< std::string > & srv)
{
	this->_config.setFileContent(srv);

	/*==================================*/

	_config.setPort(_config.extractPort());
	_config.setHost(_config.extractHost());
	_config.setSocket(_config.extractSocket());
	_config.setName(_config.extractName());
	_config.setErrorPages(_config.extractErrorPages());
	_config.setMaxBodySize(_config.extractMaxBodySize());

	/*==================================*/

	_config.setAllowedMethods(_config.extractAllowedMethods());

	_config.setRoot(_config.extractRoot());
	_config.setIndex(_config.extractIndex());

	/*==================================*/

	_config.printConfig();

	/*==================================*/
}

std::string	testing_data(void)
{
	// Testing data = will be removed ======================================= //
	std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	int hfd = open("./html/index.html", O_RDONLY );
	char b[30000];

	read( hfd, b, 30000);
	std::string html(b);

	std::ostringstream oss;
    oss << html.size();
	hello = hello + oss.str() + "\n" + b;
	// ====================================================================== //
	return ( hello );
}

// Public member functions ================================================== //
void	Server::run(void)
{
	int				cliSocket	= -1;
	int				nbEvents	= -1;
	int				addrlen		= 1;
	t_epollEv		cliEvents[ MAX_EVENTS ];

	// Creates sockets if they don't exist
	if ( this->_srvfd.size() == 0 )
		this->_initSrv();
	while ( 1 )
	{
		nbEvents = epoll_wait( this->_getEplFd( ), cliEvents, MAX_EVENTS, -1);
		for (int i = 0; i < nbEvents; ++i)
		{
			if ( cliEvents[i].data.fd == this->_getSrvFd( i ) )
			{
				// Accepting connections
				cliSocket = accept( this->_getSrvFd( i ), const_cast< t_sockaddr* >( &this->_getSockaddr( i ) ), reinterpret_cast<socklen_t *>(&addrlen) );
				if ( cliSocket == -1 )
					this->_log(ERROR, i, __func__, __LINE__, "accept");
				else
					this->_log(LOG, i, __func__, __LINE__, "connection established");
				// Sending data to client, example : end of a download or curl request
				if ( cliEvents[i].events & EPOLLOUT )
				{
					this->_log(LOG, i, __func__, __LINE__, "send request");
				}
				// Receive client request
				if ( cliEvents[i].events & EPOLLIN )
				{
					this->_log(LOG, i, __func__, __LINE__, "treating client request");
						//parse request
						//treat request
					this->_log(LOG, i, __func__, __LINE__, "sending data to client");
						//send data to client
					// Testing page
					send( cliSocket, ( testing_data() ).c_str(), ( testing_data() ).size(), 0 );
					//send( cliSocket, response.getResponse().c_str(), response.getResponse().size(), 0 );
				}
			}
			else
				this->_log(LOG, i, __func__, __LINE__, "connection refused");
			close( cliSocket );
			this->_log(LOG, i, __func__, __LINE__, "listening");
		}
	}
	// ====================================================================== //
	return;
}

// Private member functions ================================================= //
void	Server::_initSrv(void)
{
	size_t			i			= 0;
	int				srvfd		= -1;
	int				opt			= 1;

	while ( i < this->_getNbSrv() )
	{
		srvfd = -1;
		srvfd = socket( AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0 ); 
		if ( srvfd == -1 )
			this->_log(ERROR, i, __func__, __LINE__, "socket");
		if ( setsockopt( srvfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) ) == -1 )
			this->_log(ERROR, i, __func__, __LINE__, "setsockopt");
		if ( bind( srvfd, &this->_getSockaddr( i ), sizeof( t_sockaddr ) ) == -1 )
			this->_log(ERROR, i, __func__, __LINE__, "bind");
		this->_setSrvFd( srvfd );
		if ( epoll_ctl( this->_getEplFd(  ), EPOLL_CTL_ADD,
					this->_getSrvFd( i ), &this->_eplevs[i] )== -1)
			this->_log(ERROR, i, __func__, __LINE__, "epoll_ctl");
		try
		{
			if ( listen( this->_getSrvFd( i ), 100 ) == -1 )
			{
				this->_log(ERROR, i, __func__, __LINE__, "listen");
				if ( epoll_ctl( this->_getEplFd( ), EPOLL_CTL_DEL,
							this->_getSrvFd( i ), &this->_eplevs[i] ) == -1 )
					this->_log(ERROR, i, __func__, __LINE__, "epoll_ctl DEL");
			}
		}
		catch (std::exception & e)
		{
			std::cerr << e.what() << std::endl;
		}
		this->_log(LOG, i, __func__, __LINE__, "listening");
		i++;
	}

	return;
}

// log function ============================================================= //
void	Server::_log(const int error, int i, const char *func, const int line, const char *msg) 
{
	if ( error )
	{
		std::cerr << func << ":" << line - 1 << ":";
		perror(msg);
		//this->_flag = ERROR;
	}
	else
	{
		std::cout << "\t" << ( (i > -1) ? this->_getName( i ) : "Epoll instance" );
		std::cout << "[" << ( (i > -1) ? this->_getPort( i ) : -1 ) << "] : ";
		std::cout << msg << std::endl;
	}
	return;
}

// Getters ================================================================== //
size_t Server::_getNbSrv(void) const
{
	return ( this->_nbSrv );
}

const int & Server::_getEplFd(void) const
{
	return ( this->_eplfd );
}

const std::string &	Server::_getName(const size_t & i) const
{
	if ( i < 0 || i >= this->_getNbSrv() )
		throw WrongSize();
	return ( this->_names[i] );
}

size_t 	Server::_getPort(const size_t & i) const
{
	if ( i < 0 || i >= this->_getNbSrv() || i >= this->_ports.size() )
		throw WrongSize();
	return ( this->_ports[i] );
}

const int &	Server::_getSrvFd(const size_t & i) const
{
	if ( i < 0 || i >= this->_getNbSrv() || i >= this->_srvfd.size() )
		throw WrongSize();
	return ( this->_srvfd[i] );
}

const Server::t_sockaddr &	Server::_getSockaddr(const size_t & i) const
{
	if ( i < 0 || i >= this->_getNbSrv() || i >= this->_sockaddr.size() )
		throw WrongSize();
	return ( this->_sockaddr[i] );
}

const Server::t_epollEv &	Server::_getEpollEv(const size_t & i) const
{
	if ( i < 0 || i >= this->_getNbSrv() || i >= this->_eplevs.size() )
		throw WrongSize();
	return ( this->_eplevs[i] );
}

// Setters ================================================================== //
void	Server::_setName(const std::string & name)
{
	size_t	i	= 0;

	while ( i < this->_names.size() && i < this->_getNbSrv() )
	{
		if ( name == this->_names[i] )
			throw Duplicate();
		i++;
	}
	this->_names.push_back( name );
	return;
}

void	Server::_setPort(const int & port)
{
	size_t	i	= 0;

	while ( i < this->_ports.size() && i < this->_getNbSrv() )
	{
		if ( port == this->_ports[i] )
			throw Duplicate();
		i++;
	}
	this->_ports.push_back( port );
	return;
}

void	Server::_setSrvFd(const int & fd)
{
	size_t	i	= 0;

	while ( i < this->_srvfd.size() && i < this->_getNbSrv() )
	{
		if ( fd == this->_srvfd[i] )
			throw Duplicate();
		i++;
	}
	this->_eplevs[i].data.fd = fd;
	this->_srvfd.push_back( fd );
	return;
}

void	Server::_setSockaddr(void)
{
	size_t			i		= 0;
	t_sockaddr_in	addrIn;
	t_sockaddr		*addr;

	addrIn.sin_family		= AF_INET;
	addrIn.sin_addr.s_addr	= INADDR_ANY;
	for (size_t j = 0; j < sizeof( addrIn.sin_zero ); j++)
	{
		addrIn.sin_zero[j] = '\0';
	}
	while ( i < this->_getNbSrv() )
	{
		addrIn.sin_port = htons( this->_getPort( i ) );
		addr = reinterpret_cast< t_sockaddr * >( &addrIn );
		this->_sockaddr.push_back( *addr );
		i++;
	}
	return;
}

void	Server::_setEplevs(void)
{
	size_t		i		= 0;
	t_epollEv	eplev;

	eplev.events	= EPOLLIN | EPOLLOUT;
	eplev.data.fd	= -1;
	while ( i < this->_getNbSrv() )
	{
		this->_eplevs.push_back( eplev );
		i++;
	}
	return;
}
