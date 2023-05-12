/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:30 by auzun             #+#    #+#             */
/*   Updated: 2023/05/12 15:56:23 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RESPONSE_HPP
# define RESPONSE_HPP

# include "../../../include/webserv.hpp"
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include "../../utils/utils.hpp"
#include "../request/Request.hpp"
#include "../../config/Config.hpp"
#include "../../cgi/CGI.hpp"

class Response
{
	public :
		Response(void);
		Response(Request & request, Config & config, int port, std::string host);
		~Response(void);


		/*Methods*/
		void	generate();
		void	GET(void);
		void	POST(void);
		void	DELETE(void);
		/*-------*/

		void		setRequest(Request &request);
		void		setConfig(Config &config);
		void		setPath();

		std::string	getUploadFileName();
		std::string	getUploadPath();
		std::string	getResponse();

		/*Response Utils*/
		int	readContent(void);
		std::string	readErrorPage(const std::string & path);
		int	writeContent(std::string content);
		int	fileExist(std::string path);
		int	isFile(std::string path);
		int	isDir(std::string path);
		
		void		updateContentIfBoundary();
		bool		findCGI();
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

		int			_port;
		std::string	_host;

		std::string	_path;

		/*Upload*/
		std::string	_uploadFileName;

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