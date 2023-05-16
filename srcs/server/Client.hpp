/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/05/16 21:45:11 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

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
										Client( void );
										Client( const int & eplfd );
										~Client( void );

		void							add2epoll( const int & socket );
		void							close( const int & socket );
		
		//const int 					&	setFlag( const t_flag flag );

		//const int 					&	getFlag( const int socket );


	private:
		int								_eplfd;
		std::vector	< int			>	_socket;
		//std::map	< int, t_flag	>	_flag	__attribute__((unused));
		std::map	< int, char *	>	_buffer	__attribute__((unused));

};

#endif
