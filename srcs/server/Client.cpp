/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halvarez <halvarez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:22:29 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/17 09:50:10 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <unistd.h>

#include "../config/Config.hpp"
#include "Client.hpp"

// Constructors ============================================================= //
Client::Client( void ) : _eplfd( ), _socket( ), /*_flag( ),*/ _buffer(  ) 
{
	return;
}

Client::Client( const int & eplfd ) : _eplfd( eplfd ), _socket( ), /*_flag( ),*/ _buffer(  ) 
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
	return;
}

//// Public member functions ===================================================
// Socket management -----------------------------------------------------------
bool	Client::add( const int & socket )
{
	struct epoll_event	ev;
	bool				is_set	= this->setSocket( socket );

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	if (	is_set == true
		&&	epoll_ctl( this->getEpollFd(), EPOLL_CTL_ADD, socket, &ev) != -1 )
	{
		// create flag
		// create buffer
		return ( true );
	}
	else if ( is_set == true )
	{
		this->_socket.pop_back();
		if ( close( socket ) == -1 )
			std::cer << "Error: closing client socket" << std::endl;
		return ( false );
	}
	return ( false );
}

void	Client::remove( const int & socket )
{
	std::vector< int >::iterator	it = this->_socket.find( socket );
	struct epoll_event				ev;

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	// clear response buffer
	// clear socket flags
	// remove from epoll instance
	if ( epoll_ctl( this->getEpollFd(), EPOLL_CTL_DEL, socket, &ev) == -1 )
		std::cerr << "Error: remove client socket from epoll instance failed" << std::endl;
	// remove from socket vector
	if ( it != this->_socket.end() )
		this->_socket.erase( it );
	// close socket
	if ( close( socket ) == -1 )
		std::cer << "Error: closing client socket" << std::endl;
	return;
}

// Private member functions ================================================= ///

// Setters ---------------------------------------------------------------------
bool	Client::setSocket( const int & socket )
{
	std::vector< int >::iterator	it = this->_socket.find( socket );

	if (	socket != -1
		&&	this->_socket.size() > 0
		&&	it == this->_socket.end() )
	{
		this->_socket.push_back( socket );
		return ( true );
	}
	return ( false );
}

// Getters ---------------------------------------------------------------------
const int &	Client::getEpollFd( void ) const
{
	return ( this->_eplfd );
}

// Exceptions =============================================================== ///

