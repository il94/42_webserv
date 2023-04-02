/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:30 by auzun             #+#    #+#             */
/*   Updated: 2023/04/02 07:27:57 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "../request/Request.hpp"

class Response
{
	public :
		Response(void);
		~Response(void);

		int	readContent(void);
		int	writeContent(std::string content);
		int	fileExist(std::string path);
		
		std::string		getHeader(size_t size, const std::string& path, int code, std::string type);

	
	private :
		Request	_request;
		std::string	_response;
}


#endif