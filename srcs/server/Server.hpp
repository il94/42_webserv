/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/01 17:40:11 by halvarez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP


class Server
{
	public:
		typedef		struct sockaddr			t_sockaddr;
		typedef 	struct sockaddr_in		t_sockaddr_in;

							Server(void);
							Server(const Server & srv);
							~Server(void);

		Server			 &	operator=(const Server & srv);

		void				run(void);
		void				mkSrvSocket(void);

		void				setSrvFd(const int & fd);
		const int		 &	getSrvFd(void)					const;
		t_sockaddr		 *	getSockAddr(void)				const; 

	private:
		int					_srvfd;
//		int					_epfd;
		t_sockaddr		 *	_address;

};

#endif
