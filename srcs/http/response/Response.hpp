/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:30 by auzun             #+#    #+#             */
/*   Updated: 2023/04/23 17:45:26 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RESPONSE_HPP
# define RESPONSE_HPP

# include "../../../include/webserv.hpp"
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
		void	generate();
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
		
		Location	findLocation();
		/*listing.cpp/ Listing*/
			std::string	generateAutoIndex();
		/*---------------*/

		/*Header*/
		void	setContentType(std::string path);
		void	setCode(int	code);
		void	setContentLength(size_t size);
		
		bool	isAllowedMethod();
		void	initStatusMsg();
		std::string	getStatuMsg();
		std::string	writeHeader(void);
		std::string	generateHeader(size_t size, std::string path);
		/*------*/
	
	private :

		/*Header*/
		std::string					_contentLength;
		std::string					_contentType;
		std::map<int, std::string>	_statusMsg;
		int			_code;
		/*------*/
		
		Request		_request;
		Config		_config;
		std::string	_response;
		Location	_location;
};


#endif