/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/03/31 20:20:10 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP


class Server
{
	public:
		typedef struct sockaddr		t_sockaddr;
		typedef struct sockaddr_in	t_sockaddr_in;

							Server(void);
							Server(const Server & srv);
							~Server(void);

		Server			 &	operator=(const Server & srv);

		void				run(void);
//		void				bind(void);
		void				mkSrvSocket(void);

		void				setSrvFd(const int & fd);
		const int		 &	getSrvFd(void)					const;
		//int		 			getSrvFd(void)					const;
		t_sockaddr		 *	getSockAddr(void)				const; 

	private:
		int					_srvFd;
		t_sockaddr *		_address;
		//int			_port_srv;

};

#endif
