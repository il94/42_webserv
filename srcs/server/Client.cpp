/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halvarez <halvarez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:22:29 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/17 15:58:56 by halvarez         ###   ########.fr       */
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
Client::Client( void ) : _eplfd( ), _socket( ), _flag( ), _buffer(  ) 
{
	return;
}

Client::Client( const int & eplfd ) : _eplfd( eplfd ), _socket( ), _flag( ), _buffer(  ) 
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

bool	Client::add( const int & socket )
{
	struct epoll_event	ev;
	bool				is_set	= this->setSocket( socket );

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	if (	is_set == true
		&&	fcntl( socket, F_SETFL, O_NONBLOCK ) != -1
		&&	epoll_ctl( this->getEpollFd(), EPOLL_CTL_ADD, socket, &ev) != -1
		)
	{
		// create a no error flag
		this->_flag.insert( std::pair< int, t_flag >( socket, NO_ERROR ) );
		// create an empty buffer
		this->_buffer[ socket ];
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

void	Client::remove( const int & socket )
{
	struct epoll_event								ev;
	std::vector< int >::iterator					itSock = std::find( this->_socket.begin(), this->_socket.end(), socket );
	std::map< int, t_flag >::iterator				itFlag = this->_flag.find( socket );
	std::map< int, std::vector< char * >>::iterator	itBuf  = this->_buffer.find( socket );

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	// clear socket flags
	if ( itFlag != this->_flag.end() )
		this->_flag.erase( socket );
	// clear response buffer
	if ( itBuf != this->_buffer.end() )
		this->_buffer.erase( socket );
	// remove from epoll instance
	if ( epoll_ctl( this->getEpollFd(), EPOLL_CTL_DEL, socket, &ev) == -1 )
		std::cerr << "Error: remove client socket from epoll instance failed" << std::endl;
	// remove from socket vector
	if ( itSock != this->_socket.end() )
		this->_socket.erase( itSock );
	// close socket
	if ( close( socket ) == -1 )
		std::cerr << "Error: closing client socket" << std::endl;
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

void	Client::setFlag( const int & socket, const t_flag flag )
{
	this->_flag[ socket ] = flag;
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

const t_flag &	Client::getFlag( const int & socket ) const
{
	return( this->_flag.at( socket ) );
}

// Private member functions ================================================= ///
