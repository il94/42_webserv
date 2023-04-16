/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:09:47 by auzun             #+#    #+#             */
/*   Updated: 2023/04/16 04:04:01 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "../http/request/Request.hpp"
# include <vector>
# include <algorithm>
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

		void	setArgs(std::string path);
		void	setEnv();
		void	setQueryString();

		std::string	execCGI(std::string scriptName);
	private:
		Request			_request;

		std::string			_body;
		std::vector<char*>	_args;
		char				**_env;
		char				*_querystring;
};

# endif