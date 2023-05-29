/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:22:29 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/29 11:24:43 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#include "../config/Config.hpp"
#include "Server.hpp"
#include "Client.hpp"

// Constructors ============================================================= //
Client::Client( void ) : _eplfd( ), _socket( ), _port(  ), _name(  ), _flag( ),
	_Response( ), _buffer(  ), _time( )
{
	return;
}

Client::Client( const int & eplfd ) : _eplfd( eplfd ), _socket( ), _port(  ),
	_name(  ),  _flag( ), _Response( ), _buffer(  ), _time( )
{
	return;
}

/// Destructor =============================================================== ///
Client::~Client( void )
{
	std::vector< int >::iterator	it = this->_socket.begin();

	while ( it != this->_socket.end() )
	{
		if ( *it != -1 && close( *it ) == -1 )
			std::cerr << "\tError: closing client socket failed in destructor" << std::endl;
		it++;
	}
	this->_flag.clear();
	this->_buffer.clear();
	return;
}

//// Public member functions ===================================================
// Socket management -----------------------------------------------------------
int	Client::find( const int & socket ) const
{
	std::vector< int >::const_iterator	it = std::find( this->_socket.begin(), this->_socket.end(), socket );

	if ( it != this->_socket.end() )
		return ( *it );
	return ( -1 );
}

bool	Client::add( const int & socket, const int & port, const std::string & name )
{
	struct epoll_event			ev;
	bool						is_set	= this->setSocket( socket );
	std::vector<unsigned char>	vect;

	ev.events	= EPOLLIN | EPOLLOUT;
	ev.data.fd	= socket;
	if (	is_set == true
		&&	epoll_ctl( this->getEpollFd(), EPOLL_CTL_ADD, socket, &ev) != -1
		)
	{
		this->_port.insert( std::pair< int, int >( socket, port ) );
		this->_name.insert( std::pair< int, std::string >( socket, name ) );
		this->_flag.insert( std::pair< int, int >( socket, EMPTY ) );
		this->_Response.insert( std::make_pair( socket, Response() ) );
		this->_buffer.insert( std::make_pair( socket, std::vector< ustring >() ) );
		this->_upload.insert( std::pair< int, std::vector<unsigned char> >( socket, vect ) );
		this->_time.insert( std::pair< int, clock_t >( socket, std::clock() ) );
		return ( true );
	}
	else if ( is_set == true )
	{
		this->_socket.pop_back();
		if ( close( socket ) == -1 )
			std::cerr << "\tError: closing client socket" << std::endl;
		return ( false );
	}
	return ( false );
}

void	Client::remove( int & socket )
{
	struct epoll_event									ev;
	std::vector< int >::iterator						itSock = std::find( this->_socket.begin(), this->_socket.end(), socket );
	std::map< int, int >::iterator						itPort = this->_port.find( socket );
	std::map< int, std::string >::iterator				itName = this->_name.find( socket );
	std::map< int, int >::iterator						itFlag = this->_flag.find( socket );
	std::map< int, Response >::iterator					itRes  = this->_Response.find( socket );
	std::map< int, std::vector< ustring >>::iterator	itBuf  = this->_buffer.find( socket );
	std::map< int, std::vector< unsigned char >>::iterator itUp = this->_upload.find( socket );
	std::map<int, clock_t >::iterator					itClk = this->_time.find( socket );

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	// check for flags keeping alive the socket
	if ( this->getFlag( socket ) & READ )
		return;
	// delete socket client and all associated data
	try {
		if ( itPort != this->_port.end() )
			this->_port.erase( socket );
		// delete name
		if ( itName != this->_name.end() )
			this->_name.erase( socket );
		// delete socket flags
		if ( itFlag != this->_flag.end() )
			this->_flag.erase( socket );
		// delete Response class
		if ( itRes != this->_Response.end() )
			this->_Response.erase( socket );
		// delete response buffer
		if ( itBuf != this->_buffer.end() )
			this->_buffer.erase( socket );
		// delete upload buffer
		if ( itUp != this->_upload.end() )
			this->_upload.erase( socket );
		if ( itClk != this->_time.end() )
			this->_time.erase( socket );
		// remove from epoll instance
		if ( epoll_ctl( this->getEpollFd(), EPOLL_CTL_DEL, socket, &ev) == -1 )
			std::cerr << "\tError: remove client socket from epoll instance failed" << std::endl;
		// remove from socket vector
		if ( itSock != this->_socket.end() )
			this->_socket.erase( itSock );
	} catch ( std::exception & e ) {
		std::cerr << "\t" << e.what() << std::endl;
	}
	// close socket
	if ( close( socket ) == -1 )
		std::cerr << "\tError: closing client socket" << std::endl;
	socket = -1;
	return;
}

// Response management ---------------------------------------------------------
void	Client::newResponse( const int & socket, std::string res )
{
	ustring	ures = *(reinterpret_cast< ustring * >( &res ));

	try
	{
		this->_buffer.at( socket ).push_back( ures );
		this->setFlag( socket, CONTENT );
		if ( DBG )
		{
			std::cout << "------------------------- print response -------------------------" << std::endl;
			std::cout << res << std::endl;
			std::cout << "Response size = " << res.size() << std::endl;
		}
	}
	catch ( std::exception & e )
	{
		std::cerr << "\tnewResponse error: " << e.what() << std::endl;
	}
	return;
}

Client::ustring	Client::getResponse( const int & socket )
{
	ustring								response;
	std::vector< ustring >::iterator	it;

	if ( this->_buffer.at( socket ).size() )
	{
		try {
			response = this->_buffer.at( socket ).front();
			it = this->_buffer.at( socket ).begin();
			this->_buffer.at( socket ).erase( it );
			if ( this->_buffer.at( socket ).size() == 0 )
				this->unSetFlag( socket, CONTENT );
		}
		catch ( std::exception & e ) {
			std::cerr << "\tError : couldn't access to the next response." <<std::endl;
			response.clear();
		}
	}
	return ( response );
}

size_t	Client::responseSize( const int & socket ) const
{
	return ( ( this->_buffer.at( socket ).front() ).size() );
}

void	Client::str2upload( const int & socket, std::string & str )
{
	std::string::iterator	it = str.begin();

	while ( it != str.end() )
	{
		this->_upload.at( socket ).push_back( *it );
		it++;
	}
	return;
}

// Setters ---------------------------------------------------------------------
bool	Client::setSocket( const int & socket )
{
	std::vector< int >::iterator	it = std::find( this->_socket.begin(), this->_socket.end(), socket );

	if (	socket != -1
		&&	this->_socket.size() >= 0
		&&	it == this->_socket.end() )
	{
		this->_socket.push_back( socket );
		return ( true );
	}
	return ( false );
}

void	Client::setFlag( const int & socket, const int flag )
{
	this->_flag.at( socket ) |= flag;
	return;
}

void	Client::unSetFlag( const int & socket, const int flag )
{
	this->_flag.at( socket ) &= ~flag;
	return;
}

void	Client::setClassResponse( const int & socket, const Config & conf, Request & req  )
{
	Response	res( req, conf, this->getPort( socket ), this->getName( socket ) );

	this->_Response[socket] = res;
	return;
}

void	Client::checkClock( void )
{
	int		socket;
	float	time_s;
	size_t	i		= 0;

	while ( i < this->_socket.size() )
	{
		socket = this->_socket.at( i );
		this->_time[ socket ] = std::clock() - this->_time[ socket ];	
		time_s = static_cast<float>( this->_time[ socket ] ) / CLOCKS_PER_SEC;
		if ( time_s >= TIMEOUT )
			this->remove( socket );
		//else
		//	this->_time[ socket ] = std::clock();
		i++;
	}
	return;
}

void	Client::resetClock( const int & socket )
{
	this->_time[ socket ] = std::clock();
	return;
}

// Getters ---------------------------------------------------------------------
const int &	Client::getEpollFd( void ) const
{
	return ( this->_eplfd );
}

size_t	Client::size( void ) const
{
	return ( this->_socket.size() );
}

const int &	Client::getPort( const int & socket ) const
{
	return ( this->_port.at( socket)  );
}

const std::string &	Client::getName( const int & socket ) const
{
	return ( this->_name.at( socket ) );
}

int	Client::getFlag( const int & socket ) const
{
	if (this->_flag.find(socket) == this->_flag.end())
		return (-1);
	return( this->_flag.at( socket ) );
}

Response &	Client::getClassResponse( const int & socket )
{
	return ( this->_Response.at( socket ) );
}

Response *	Client::getClassResponsePTR( const int & socket)
{
	return ( &( this->_Response.at( socket ) ) );
}

std::vector< unsigned char > &	Client::getUpload( const int & socket )
{
	return ( this->_upload.at( socket ) );
}

std::vector< unsigned char > *	Client::getUploadPTR( const int & socket )
{
	return ( &(this->_upload.at( socket )) );
}

const std::clock_t &	Client::getClock( const int & socket )
{
	return ( this->_time.at( socket ) );
}
