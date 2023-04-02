/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:27 by auzun             #+#    #+#             */
/*   Updated: 2023/04/02 22:38:37 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void) {};

Response::~Response(void) {};

/*Methods*/
void	Response::GET(Request	&request)
{
	_response = readContent();
	_response = getHeader(_response.size(), request.getURL(), 200) + "\r\n" + _response;
}
/*-------*/

/*Response Utils*/
int	Response::readContent(void)
{
	std::ifstream	file;
	std::stringstream	buffer;
	std::string	path = _request.getURL();

	if (fileExist(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			return (-1);
		}
		
		buffer << file.rdbuf();
		_response = buffer.str();
		file.close();
		return (0);
	}
	return (-1);
}

int	Response::writeContent(std::string content)
{
	std::ofstream	file;
	std::string	path = _request.getURL();

	if (fileExist(path))
	{
		file.open(path.c_str());
		if (file.is_open() == false)
			return (-1);
		file << content;
		file.close();
		return (0);
	}
	return (-1);
}


int	Response::fileExist(std::string path)
{
	struct stat	stats;

	if (stat(path.c_str(), &stats) == 0)
		return 1;
	return 0;
}
/*---------------*/

/*Header*/
std::string	Response::getHeader(size_t size, std::string path, int code)
{
	std::string	header;

	setContentLength(size);
	setContentType(path);
	setCode(code);
	header = writeHeader();
	return (header);
}

std::string	Response::writeHeader(void)
{
	std::string	header = "HTTP/1.1 " + _code + " " + "ok" + "\r\n";

	if (!_contentLength.empty())
		header += "Content-Length: " + _contentLength + "\r\n";
	if (!_contentType.empty())
		header += "Content-Type: " + _contentType + "\r\n";
	return (header);
}

void	Response::setCode(int code)
{
	_code = to_string(code);
}

void	Response::setContentLength(size_t size)
{
	_contentLength = to_string(size);
}

void	Response::setContentType(std::string path)
{
	std::string	type = path.substr(path.find(".") + 1);
	if (type == "html")
		_contentType = "text/html";
	else if (type == "css")
		_contentType = "text/css";
	else if (type == "js")
		_contentType = "text/javascript";
	else if (type == "jpeg" || type == "jpg")
		_contentType = "image/jpeg";
	else if (type == "png")
		_contentType = "image/png";
	else if (type == "bmp")
		_contentType = "image/bmp";
	else
		_contentType = "text/plain";

}
/*------*/