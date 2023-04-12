/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:54:50 by halvarez          #+#    #+#             */
/*   Updated: 2023/04/11 15:30:38 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <map>
#include <vector>

#include "../config_file/Config.hpp"

class Server
{
	public:
					Server(void);
					Server( const std::string & );
					Server(const Server & srv);
					~Server(void);

		Server &	operator=(const Server & srv);

		// void		run(void);
		void		setConfig(std::vector< std::string > & srv);

	private:
		Config	config;
};

#endif
