/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/02 16:24:46 by halvarez         ###   ########.fr       */
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

							Server(void);
							Server(const Server & srv);
							~Server(void);

		Server			 &	operator=(const Server & srv);

		void				run(void);
		void				mkSrvSocket(void);

		void				setFd(const t_fd FD, const int & fd);
		const int		 &	getFd(const t_fd FD)					const;
		t_sockaddr		 *	getSockAddr(void)						const; 

	private:
		int					_srvfd;
		int					_eplfd __attribute__((unused));
		t_sockaddr		 *	_address;

};

#endif
