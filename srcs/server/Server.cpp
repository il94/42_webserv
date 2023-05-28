/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:26:30 by ilandols          #+#    #+#             */
/*   Updated: 2023/05/28 23:39:41 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#define MAX_EVENTS	128

// Constructors ============================================================= //
Server::Server(void) : _nbSrv( 0 ), _names( ), _ports( ), _sockaddr( ), _eplevs( ), _srvfd( )
{
	this->_eplfd = epoll_create( 1 );
	if ( this->_eplfd == -1 )
		this->_displayError( __func__, __LINE__, "Server/epoll_create" );
	if ( fcntl( this->_eplfd, F_SETFL, O_NONBLOCK) == -1 )
		this->_displayError( __func__, __LINE__, "Server/fcntl" );
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
		this->_displayError( __func__, __LINE__, "Server/epoll_create" );
	if ( fcntl( this->_eplfd, F_SETFL, O_NONBLOCK) == -1 )
		this->_displayError( __func__, __LINE__, "Server/fcntl" );
	return;
}

// Destructor =============================================================== //
Server::~Server(void)
{
	size_t	i = 0;

	while ( i < this->_getNbSrv() )
	{
		if ( close( this->_getSrvFd( i ) ) == -1 )
			this->_displayError( __func__, __LINE__, "~Server/close" );
		i++;
	}
	if ( close( this->_getEplFd() ) == -1 )
		this->_displayError( __func__, __LINE__, "~Server/close" );
	return;
}

// Operators ================================================================ //
Server &	Server::operator=(const Server & srv )
{
	size_t	i	= 0;

	if ( this->_srvfd.size() != 0 )
		for (size_t j = 0; j < this->_srvfd.size(); j++)
		{
			close( this->_getSrvFd( j ) );
		}
	this->_srvfd.clear();
	this->_names	= srv._names;
	this->_ports	= srv._ports;
	this->_sockaddr	= srv._sockaddr;
	this->_srvfd	= srv._srvfd;
	this->_nbSrv	= srv._ports.size();
	while ( i < this->_getNbSrv() )
	{
		this->_srvfd.push_back( dup( srv._getSrvFd( i ) ) );
		this->_eplevs[i].data.fd = this->_getSrvFd( i );
		i++;
	}
	this->_eplfd = epoll_create( 1 );
	if ( this->_eplfd == -1 )
		this->_displayError( __func__, __LINE__, "Operator=/epoll_create" );
	return ( *this );
}

// Public member functions ================================================== //
void	Server::display(void)
{
	std::cout << std::endl << "========================================" << std::endl << std::endl;

	std::cout << _getNbSrv() << " Instances" << std::endl << std::endl; 

	displayVector(_ports, "Ports alloues");

	std::cout << std::endl;	

	displayVector(_names, "Serveur concerne");

	std::cout << std::endl;

	std::cout << "========================================" << std::endl << std::endl;

	for (std::vector<Config>::iterator it = _configs.begin(); it != _configs.end(); it++)
	{
		std::cout << "Serveur [" << it->getName() << "]" << std::endl << std::endl;
		it->display();
		std::cout << "========================================" << std::endl << std::endl;
	}
}

extern bool running;

std::vector<std::vector <std::string> >	Server::extractContent( const std::string &path )
{
	std::vector<std::vector <std::string> > result;

	std::vector<std::string>			fileContent = fileToVector(path);
	std::vector<std::string>::iterator	start;
	std::vector<std::string>::iterator	end;

	std::vector<std::string>			element;

	for (start = fileContent.begin(); start != fileContent.end(); start = end)
	{
		element.clear();
		end = start + 1;
		if (static_cast<int>(start->rfind("server", 0, sizeof("server") - 1)) != -1 and openBrace(*start, start->find("server", 0) + sizeof("server")))
		{
			while (end != fileContent.end() and (*end)[0] != '}')
			{
				element.push_back(*end);
				end++;
			}
			if (end != fileContent.end() and (*end)[0] == '}')
				result.push_back(element);
		}
	}
	return (result);
}

void	Server::setConfigs( char **av )
{
	std::string	src;
	if (av[0] and av[1])
		src = av[1];

	setContent(extractContent(src));

	bool	error = false;

	for (std::vector<std::vector <std::string> >::iterator it = _content.begin(); it != _content.end(); it++)
	{
		Config	tmp;

		tmp.setContent(tmp.extractContent(*it));

		tmp.setPort(tmp.extractPort());
		tmp.setHost(tmp.extractHost());
		tmp.setName(tmp.extractName());
		tmp.setMaxBodySize(tmp.extractMaxBodySize());

		tmp.setDefaultLocation(tmp.extractDefaultLocation());
		tmp.setErrorPages(tmp.extractErrorPages());
		tmp.setLocations(tmp.extractLocations(*it));

		if (tmp.getError() == false)
		{
			std::vector<int>	tmpPorts = tmp.getPort();

			_ports.insert(_ports.end(), tmpPorts.begin(), tmpPorts.end());
			for (size_t i = 0; i < tmpPorts.size(); i++)
				_names.push_back(tmp.getName());

			_configs.push_back(tmp);
		}
		else
			error = true;
	}
	if (_configs.empty() == true and error == false and running)
	{
		_configs.push_back(Config());
		_names.push_back(DEFAULT_NAME);
		_ports.push_back(DEFAULT_PORT);
	}
	for (size_t i = 0; i < _ports.size(); i++)
	{
		for (size_t j = i + 1; j < _ports.size(); j++)
		{
			if (_ports[j] == _ports[i])
			{
				_ports.erase(_ports.begin() + j);
				_names.erase(_names.begin() + j);
				j--;
			}
		}
	}

	_nbSrv = _ports.size();
}

void	Server::run(void)
{
	int				cliSocket	= -1;
	Client			client( this->_getEplFd() );
	int				nbEvents	= -1;
	t_epollEv		cliEvents[ MAX_EVENTS ];
	std::string		request;
	size_t			sizeRequest	= 1000000;
	int				size = 0;

	std::cout << "Server log : " << std::endl;
	// Creates sockets if they don't exist
	if ( this->_srvfd.size() == 0 )
		this->_initSrv();
	while ( running )
	{
		nbEvents = epoll_wait( this->_getEplFd( ), cliEvents, MAX_EVENTS, TIMEOUT );
		if ( nbEvents == -1 )
			this->_displayError( __func__, __LINE__, "what)run/epoll_wait" );
		client.checkClock();
		for (int i = 0; running and i < nbEvents; i++)
		{
			cliSocket = -1;
			for ( size_t j = 0; running and j < client.size(); j++ )
			{
				if ( client.find( cliEvents[i].data.fd ) != -1 )
				{
					cliSocket = cliEvents[i].data.fd;
					client.resetClock( cliSocket );
					if ( cliSocket != -1 && cliEvents[i].events & EPOLLOUT && client.getFlag( cliSocket ) == CONTENT )
					{
						// send response
						this->_sendResponse( cliSocket, client, size );
						client.remove( cliSocket );
					}
					if ( cliSocket != -1 && cliEvents[i].events & EPOLLIN )
					{
						// clean local request buffer
						request.clear();
						request.resize( sizeRequest );
						if ( cliSocket != -1 && cliEvents[i].events & EPOLLIN )
						{
							// read the request
							request = this->_readRequest( client, cliSocket, request );
							// parse request and generate response
							if ( cliSocket != -1 && request.size() > 0 )
							{
								size = this->_storeResponse( client, cliSocket, request );
							}
						}
					}
				}
			}
			for (size_t k = 0; running and k < this->_getNbSrv(); k++)
			{
				if ( cliEvents[i].data.fd == this->_getSrvFd( k ) )
				{
					cliSocket = this->_acceptConnection( k, client );
					cliSocket = -1;
				}
			}
		}
	}
	return;
}

// Private member functions ================================================= //
void	Server::_initSrv(void)
{
	size_t			i			= 0;
	int				srvfd		= -1;
	int				opt			= 1;

	if ( this->_sockaddr.size() == 0 )
		this->_setSockaddr();
	if ( this->_eplevs.size() == 0 )
		this->_setEplevs();
	while ( i < this->_getNbSrv() )
	{
		srvfd = -1;
		srvfd = socket( AF_INET, SOCK_STREAM, 0 ); 
		if ( srvfd == -1  || fcntl( srvfd, F_SETFL, O_NONBLOCK ) == -1 )
			this->_displayError( __func__, __LINE__, "_initSrv/socket" );
		if ( setsockopt( srvfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt) ) == -1 )
			this->_displayError( __func__, __LINE__, "_initSrv/setsockopt" );
		if ( bind( srvfd, &this->_getSockaddr( i ), sizeof( t_sockaddr ) ) == -1 )
		{
			this->_displayError( __func__, __LINE__, "_initSrv/bind" );
			this->_names.erase( this->_names.begin() + i );
			this->_ports.erase( this->_ports.begin() + i );
			this->_sockaddr.erase( this->_sockaddr.begin() + i );
			this->_eplevs.erase( this->_eplevs.begin() + i );
			this->_nbSrv--;
			if ( srvfd != -1 )
				close( srvfd );
		}
		else
		{
			this->_setSrvFd( srvfd );
			if ( epoll_ctl( this->_getEplFd(  ), EPOLL_CTL_ADD, this->_getSrvFd( i ), &this->_eplevs[i] )== -1)
				this->_displayError( __func__, __LINE__, "_initSrv/epoll_ctl ADD" );
			try
			{
				if ( listen( this->_getSrvFd( i ), 100 ) == -1 )
				{
					this->_displayError( __func__, __LINE__, "_initSrv/listen" );
					if ( epoll_ctl( this->_getEplFd(), EPOLL_CTL_DEL, this->_getSrvFd( i ), &this->_eplevs[i] ) == -1 )
						this->_displayError( __func__, __LINE__, "_initSrv/epoll_ctl DEL" );
				}
				std::cout << "\t" << this->_getName( i ) << "[" << this->_getPort( i ) << "]\t: ";
				std::cout << "listening" << std::endl;
			}
			catch (std::exception & e)
			{
				std::cerr << e.what() << std::endl;
			}
			i++;
		}
	}
	return;
}

int	Server::_acceptConnection(const int & j, Client & client )
{
	int			cliSocket;
	socklen_t	addrlen		= sizeof( t_sockaddr );

	cliSocket = accept( this->_getSrvFd( j ), const_cast< t_sockaddr* >( &this->_getSockaddr( j ) ), &addrlen );
	if ( cliSocket == -1 )
		this->_displayError( __func__, __LINE__, "_acceptConnection/accept" );
	else
	{
		if ( client.add( cliSocket, this->_getPort( j ), this->_getName( j ) ) == false )
		{
			std::cerr << "Error: adding socket bind on " << this->_getPort( j ) << " failed." <<std::endl;
			if ( close( cliSocket ) == -1 )
				this->_displayError( __func__, __LINE__, "_acceptConnection/close" );
			return ( -1 );
		}
	}
	return( cliSocket );
}

std::string &	Server::_readRequest(Client & client, int & cliSocket, std::string & request)
{
	int			bytes = 0;
	std::string	strUpload;

	bytes = recv(cliSocket, reinterpret_cast<void*>(const_cast<char*>(request.data())), request.size() - 1, 0);
	if ( ( bytes == 0 || bytes == -1 ) && ( client.getFlag( cliSocket ) & READ ) == 0 )
	{
		this->_displayError( __func__, __LINE__, "_readRequest/recv" );
		client.remove( cliSocket );
		cliSocket = -1;
		std::cerr << "\tRead bytes = " << bytes << std::endl;
		return ( request );
	}
	else
	{
		request[ bytes ] = '\0';
		request.resize( bytes );
	}
	if ( cliSocket != -1 && ( client.getFlag( cliSocket ) & READ || request.find( "\r\n\r\n" ) != std::string::npos ) )
	{
		if (client.getFlag( cliSocket ) & READ)
			strUpload = request;
		else
			strUpload = request.substr( request.find( "\r\n\r\n" ) + 4 );
		client.str2upload( cliSocket, strUpload );
	}
	if ( DBG )
	{
		std::cout << "------------------------- print request -------------------------" << std::endl;
		std::cout << request << std::endl;
	}
	return ( request );
}

int	Server::_storeResponse( Client & client, const int & cliSocket, std::string & request )
{
	Request		req;
	Response	*rep = NULL;

	if ( DBG )
		std::cout << YELLOW << request << END << std::endl;
	if ( client.getFlag( cliSocket ) == EMPTY )
	{
		req.parseHeader(request);
		if (req.getRet() == 200)
			req.parseBody();
		// client.setFlag( cliSocket, flag_value );
		int port = client.getPort(cliSocket);
		client.setClassResponse( cliSocket, getConfig(port), req);
	}

	rep = client.getClassResponsePTR( cliSocket );
	if (rep == NULL)
	{
		running = false;
		return (0);
	}
	
	(*rep).setContent( client.getUploadPTR( cliSocket ));

	(*rep).generate();

	if (running == false)
		return (0);

	client.unSetFlag(cliSocket, READ);
	client.setFlag(cliSocket, (*rep).getUploadStatu());
	if ( client.getFlag( cliSocket ) == EMPTY || client.getFlag( cliSocket ) & STOP )
		client.newResponse( cliSocket, (*rep).getResponse() );
	return ( (*rep).getResponse().size() );
}

void	Server::_sendResponse( int & cliSocket, Client & client, size_t size )
{
	int		bytes = 0;
	size_t	sizeResp = client.responseSize( cliSocket );

	bytes = send( cliSocket,
			static_cast< const unsigned char* >( ( client.getResponse( cliSocket ) ).c_str() ),
			sizeResp,
			0 );
	if ( bytes == -1 || static_cast< size_t >( bytes ) != sizeResp || size != static_cast< size_t >( bytes ) )
	{
		client.remove( cliSocket );
		std::cerr << "\tError: send response to client failed, ";
		std::cerr << "\tthe socket has been closed and cleared." << std::endl;
	}
	return;
}

// log function ============================================================= //
void	Server::_displayError(const char *func, const int line, const char *msg) 
{
	std::cerr << "\t" << func << ":" << line - 1 << ":";
	perror(msg);
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
	if ( i >= this->_getNbSrv() )
		throw WrongSize();
	return ( this->_names[i] );
}

size_t 	Server::_getPort(const size_t & i) const
{
	if ( i >= this->_getNbSrv() || i >= this->_ports.size() )
		throw WrongSize();
	return ( this->_ports[i] );
}

const int &	Server::_getSrvFd(const size_t & i) const
{
	if ( i >= this->_getNbSrv() || i >= this->_srvfd.size() )
		throw WrongSize();
	return ( this->_srvfd[i] );
}

const Server::t_sockaddr &	Server::_getSockaddr(const size_t & i) const
{
	if ( i >= this->_getNbSrv() || i >= this->_sockaddr.size() )
		throw WrongSize();
	return ( this->_sockaddr[i] );
}

const Server::t_epollEv &	Server::_getEpollEv(const size_t & i) const
{
	if ( i >= this->_getNbSrv() || i >= this->_eplevs.size() )
		throw WrongSize();
	return ( this->_eplevs[i] );
}

std::vector<std::vector <std::string> >	Server::getContent( void ) const {
	return (_content);
}

typedef	std::vector	< Config >	t_vConfig;
typedef	std::vector < int >		t_vInt;

t_vConfig	Server::getConfigs( void )	const {
	return (_configs);
}

Config	Server::getConfig( int & port )	const
{
	t_vConfig	configs = getConfigs();
	t_vInt		ports;
	t_vConfig::iterator it = configs.begin();

	while (it != configs.end())
	{
		ports = it->getPort();
		if (std::find(ports.begin(), ports.end(), port) != ports.end())
			break ;
		it++;
	}
	return (*it);
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

void	Server::setContent( const std::vector<std::vector <std::string> > &src )
{
	_content = src;
}
