/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epoll_serv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halvarez <halvarez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 18:00:58 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/06 18:09:25 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
//#include <string>
//#include <cstdio>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>

int main(void)
{
	// socket creation ====================================================== //
	static t_sockaddr_in	address;
	int	listenfd	= -1;
	int	opt = 1;

	address.sin_family		= AF_INET;
	address.sin_addr.s_addr	= INADDR_ANY;
	address.sin_port		= htons( PORT );
	for (size_t i = 0; i < sizeof( address.sin_zero ); i++)
	{
		address.sin_zero[i] = '\0';
	}
	this->_address = reinterpret_cast<t_sockaddr *>(&address);

	listenfd = socket( AF_INET, SOCK_STREAM /*| SOCK_NONBLOCK*/, 0 ); 
	if (ilistenfd == -1)
		perror("socket");

	if ( setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) ) == -1 )
		perror("setsocktop")

	if ( bind( listenfd, this->_getSockAddr(), sizeof( t_sockaddr ) ) == -1 )
		perror("bind");
	if ( listen( listenfd, 100 ) == -1 )
		perror("listen");
	// ====================================================================== //

	// set epoll ============================================================ //
	static t_epoll_event	eplev;
	int						epollfd	= -1;

	eplev.events = EPOLLIN | EPOLLOUT | EPOLLHUP;
	eplev.data.fd = listenfd;

	epollfd = epoll_create( 1 );
	if ( epollfd == -1 )
		this->_srvError(__func__, __LINE__, "epoll_create");
	// ====================================================================== //
	// infinite loop ======================================================== //
	while ( 1 )
	{
		std::cout << "========== waiting for connection ==========" << std::endl;
		nbEvents = epoll_wait( this->_getFd( EPL ), cliEvents, MAX_EVENTS, 5000);
		std::cout << "============================================" << std::endl;
		cliSocket = accept( this->_getFd( SRV ), this->_address, reinterpret_cast<socklen_t *>(&addrlen) );
		if ( cliSocket == -1 )
			this->_srvError(__func__, __LINE__, "accept");
		recv( cliSocket, buffer, 1000, 0 );
		send( cliSocket, hello.c_str(), hello.size(), 0 );
		
	}
}
