/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:09:47 by auzun             #+#    #+#             */
/*   Updated: 2023/04/12 21:58:57 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "../http/request/Request.hpp"
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <stdlib.h>

# define	CGI_BUFSIZE 42424

class CGI
{
	public:
		CGI();
		CGI(Request &request);
		~CGI();

		std::string	execCGI(std::string scriptName);
	private:
		std::string		_body;
};

# endif