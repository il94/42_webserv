/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/16 22:16:47 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/epoll.h>

#include "../config/Config.hpp"
#include "Server.hpp"

/*
typedef enum e_flag
{
	LOG			= 0,
	NO_ERROR	= 0,
	ERROR		= 1 << 0
}			t_flag;
*/

class Client
{
	public:
		// Constructor ---------------------------------------------------------
										Client( const int & eplfd );
		// Destructor ----------------------------------------------------------
										~Client( void );

		// Socket management ---------------------------------------------------
		void							add( const int & socket );
		void							remove( const int & socket );
		void							close( const int & socket );
		
		// Response management -------------------------------------------------
		

		// Setters -------------------------------------------------------------
		void								setEpollFd( const int & eplfd );
		//const int 					&	setFlag( const t_flag flag );

		// Getters -------------------------------------------------------------
		int									getEpollFd( void )				const;
		//const int 					&	getFlag( const int socket );


	private:
		// Private attributes --------------------------------------------------
		int								_eplfd;
		std::vector	< int			>	_socket;
		//std::map	< int, t_flag	>	_flag	__attribute__((unused));
		std::map	< int, char *	>	_buffer	__attribute__((unused));

		// Private functions ---------------------------------------------------
										Client( void );

};

#endif
