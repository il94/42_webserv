/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/02 18:28:06 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

typedef enum e_fd
{
	SRV,
	EPL
}			t_fd;

class Server
{
	public:
		typedef		struct sockaddr			t_sockaddr;
		typedef 	struct sockaddr_in		t_sockaddr_in;
		typedef		struct epoll_event		t_epoll_event;

							Server(void);
							Server(const Server & srv);
							~Server(void);

		Server			 &	operator=(const Server & srv);

		void				run(void);

	private:
		int					_srvfd;
		int					_eplfd; 
		t_sockaddr		 *	_address;
		t_epoll_event	 *	_eplev;

		void				_setSockAddr(void);
		void				_mkSrvSocket(void);
		void				_mkEpoll(void);
		const int		 &	_getFd(const t_fd FD)				const;
		t_sockaddr 		 *	_getSockAddr(void)					const; 

};

#endif
