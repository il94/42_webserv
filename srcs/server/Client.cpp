/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halvarez <halvarez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:22:29 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/16 22:23:14 by halvarez         ###   ########.fr       */
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
		if ( *it != -1 && ::close( *it ) == -1 )
			std::cerr << "Error: closing client socket in destructor" << std::endl;
		it++;
	}
	return;
}

//// Public member functions ================================================== ///
void	Client::add( const int & socket )
{
	struct epoll_event ev;

	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = socket;
	if (	socket != -1
		&&	epoll_ctl( this->getEpollFd(), EPOLL_CTL_ADD, socket, &ev) == -1 )
	{
		std::cerr << "Error: add socket to epoll instance failed" << std::endl;
		::close( socket );
	}

}

// Private member functions ================================================= ///

// Setters ---------------------------------------------------------------------
void	Client::setEpollFd( const int & eplfd )
{
	this->_eplfd = eplfd;
	return;
}

// Getters ---------------------------------------------------------------------
int	Client::getEpollFd( void ) const
{
	return ( this->_eplfd );
}

// Exceptions =============================================================== ///

