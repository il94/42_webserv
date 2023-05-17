/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/17 12:25:47 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/epoll.h>

#include "../config/Config.hpp"
#include "Server.hpp"

typedef enum e_flag
{
	LOG			= 0,
	NO_ERROR	= 0,
	ERROR		= 1 << 0
}			t_flag;

class Client
{
	public:
		// Constructor ---------------------------------------------------------
										Client( const int & eplfd );
		// Destructor ----------------------------------------------------------
										~Client( void );

		// Socket management ---------------------------------------------------
		bool							add( const int & socket );
		void							remove( const int & socket );
		
		// Response management -------------------------------------------------
		// void							newResponse( const int & socket, const Response rep );
		// Response						getResponse( const int & socket );

		// Setters -------------------------------------------------------------
		bool							setSocket( const int & socket );
		void							setFlag( const int & socket, const t_flag flag );

		// Getters -------------------------------------------------------------
		const int					&	getEpollFd( void )				const;
		const t_flag				&	getFlag( const int & socket )	const;

	private:
		// Private attributes --------------------------------------------------
		const int										_eplfd;
		std::vector	< int							>	_socket;
		std::map	< int, t_flag					>	_flag;
		std::map	< int, std::vector< char *	>	>	_buffer;

		// Private functions ---------------------------------------------------
										Client( void );

};

#endif
