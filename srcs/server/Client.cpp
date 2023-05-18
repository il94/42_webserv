/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halvarez <halvarez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:22:29 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/18 12:28:41 by halvarez         ###   ########.fr       */
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
Client::Client( void ) : _eplfd( ), _socket( ), _port(  ), _name(  ), _flag( ), _buffer(  ) 
{
	return;
}

Client::Client( const int & eplfd ) : _eplfd( eplfd ), _socket( ), _port(  ), _name(  ),  _flag( ), _buffer(  ) 
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
			std::cerr << "Error: closing client socket failed in destructor" << std::endl;
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
	struct epoll_event	ev;
	bool				is_set	= this->setSocket( socket );

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	if (	is_set == true
		//&&	fcntl( socket, F_SETFL, O_NONBLOCK ) != -1
		&&	epoll_ctl( this->getEpollFd(), EPOLL_CTL_ADD, socket, &ev) != -1
		)
	{
		this->_port.insert( std::pair< int, int >( socket, port ) );
		this->_name.insert( std::pair< int, std::string >( socket, name ) );
		this->_flag.insert( std::pair< int, int >( socket, EMPTY ) );
		this->_buffer.insert( std::make_pair( socket, std::vector< ustring >() ) );
		return ( true );
	}
	else if ( is_set == true )
	{
		this->_socket.pop_back();
		if ( close( socket ) == -1 )
			std::cerr << "Error: closing client socket" << std::endl;
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
	std::map< int, std::vector< ustring >>::iterator	itBuf  = this->_buffer.find( socket );

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	// delete port
	try {
		if ( itPort != this->_port.end() )
			this->_port.erase( socket );
		// delete name
		if ( itName != this->_name.end() )
			this->_name.erase( socket );
		// delete socket flags
		if ( itFlag != this->_flag.end() )
			this->_flag.erase( socket );
		// delete response buffer
		if ( itBuf != this->_buffer.end() )
			this->_buffer.erase( socket );
		// remove from epoll instance
		if ( epoll_ctl( this->getEpollFd(), EPOLL_CTL_DEL, socket, &ev) == -1 )
			std::cerr << "Error: remove client socket from epoll instance failed" << std::endl;
		// remove from socket vector
		if ( itSock != this->_socket.end() )
			this->_socket.erase( itSock );
	} catch ( std::exception & e ) {
		std::cerr << e.what() << std::endl;
	}
	// close socket
	if ( close( socket ) == -1 )
		std::cerr << "Error: closing client socket" << std::endl;
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
		std::cout << "------------------------- print response -------------------------" << std::endl;
		std::cout << res << std::endl;
	}
	catch ( std::exception & e )
	{
		std::cerr << "newResponse error: " << e.what() << std::endl;
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
				this->setFlag( socket, ~CONTENT );
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

const int &	Client::getFlag( const int & socket ) const
{
	return( this->_flag.at( socket ) );
}

// Private member functions ================================================= ///
