/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/13 17:51:37 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>

typedef enum e_fd
{
	SRV,
	EPL
}			t_fd;

typedef enum e_flag
{
	EMPTY = 0,
	ERROR = 1 << 0
}	t_flag;

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
		std::string			_name;
		int					_port;
		int					_srvfd;
		int					_eplfd; 
		t_sockaddr		 *	_address;
		t_epoll_event	 *	_eplev;
		t_flag				_flag;

		void				_srvError(const char *func, const int line,
										const char *msg);
		void				_log(const char *msg)				const;
		void				_setSockAddr(void);
		void				_mkSrvSocket(void);
		void				_setEpollEvent(void);
		void				_mkEpoll(void);
		const int		 &	_getFd(const t_fd FD)				const;
		t_sockaddr 		 *	_getSockAddr(void)					const;
		t_epoll_event	 *  _getEpollEvent(void)				const;
		void				_setPort(const int port);
		const int 		 &	_getPort(void) 						const;
		void				_setName(const std::string name);
		const std::string &	_getName(void)						const;

};

#endif
