/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:30 by auzun             #+#    #+#             */
/*   Updated: 2023/04/18 21:47:01 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "../../cgi/CGI.hpp"
#include "../../utils/utils.hpp"
#include "../request/Request.hpp"
#include "../../config/Config.hpp"

class Response
{
	public :
		Response(void);
		Response(Request & request, Config & config);
		~Response(void);

		/*Methods*/
		void	GET(void);
		void	POST(void);
		void	DELETE(void);
		/*-------*/

		void	setRequest(Request &request);
		std::string	getResponse();

		/*Response Utils*/
		int	readContent(void);
		std::string	readErrorPage(const std::string & path);
		int	writeContent(std::string content);
		int	fileExist(std::string path);
		/*---------------*/

		/*Header*/
		void	setContentType(std::string path);
		void	setCode(int	code);
		void	setContentLength(size_t size);
		
		std::string	getStatuMsg();
		void	initStatusMsg();
		std::string	writeHeader(void);
		std::string	getHeader(size_t size, std::string path);
		/*------*/
	
	private :

		/*Header*/
		std::string	_contentLength;
		std::string	_contentType;
		std::map<int, std::string> _statusMsg;
		int			_code;
		/*------*/
		
		Request		_request;
		Config		_config;
		std::string	_response;
};


#endif