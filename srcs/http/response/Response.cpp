/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:27 by auzun             #+#    #+#             */
/*   Updated: 2023/04/21 04:57:24 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void): _contentLength(""), _contentType(""), _code(200), _response("") {}

Response::Response(Request & request, Config & config) :
	_contentLength(""), _contentType(""), _code(200),
		_request(request), _config(config), _response(""){}

Response::~Response(void) {}

/*Methods*/
void	Response::GET(void)
{
	if (_request.getMethod() != "GET")
		return ;
	if (_code == 200)
		_code = readContent();
	else
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = getHeader(_response.size(), _request.getURL()) + "\r\n" + _response;
}

void	Response::POST(void)
{
	if (_request.getMethod() != "POST")
		return ;
	if (fileExist("./html/cgi_test/" + _request.getURL()))
	{
		CGI cgi(_request);
		_response = cgi.execCGI("./html/cgi_test/" + _request.getURL());
		while (!_response.empty() && (_response[0] == '\n' || _response[0] == '\r'))
			_response.erase(0, 1);
		size_t	bodyPosition = _response.find("\r\n\r\n");
		size_t	boundary = std::string::npos;

		std::string			tmp;
		std::istringstream	stream(_response);
		
		while (std::getline(stream, tmp))
		{
			if (tmp == "")
				break;
			boundary = tmp.find(":");
			if (boundary != std::string::npos)
			{
				if (boundary > bodyPosition)
					break;
				std::string	key(tmp, 0, boundary);
				std::string	value(tmp, boundary + 2);
				if (key == "Status")
					_code = std::atoi(value.c_str());
				else if (key == "Content-Type")
					_contentType = value;
			}
		}
		_response = _response.substr(bodyPosition + 2);
	}
	else
	{
		_code = 204;
		_response = "";
	}
	if (_code != 200)
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = getHeader(_response.size(), "") + _response;
}

void	Response::DELETE(void)
{
	if (fileExist(_request.getURL()))
	{
		if (remove(_request.getURL().c_str()) == 0)
			_code = 204;
		else
			_code = 403;
	}
	else
		_code = 404;
	if (_code == 404 || _code == 403)
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = getHeader(_response.size(), _request.getURL()) + "\r\n" + _response;
}

/*-------*/

/*Response Utils*/
int	Response::readContent(void)
{
	std::ifstream	file;
	std::stringstream	buffer;
	std::string	path = _request.getURL();

	_response = "";

	if (fileExist(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_response = readErrorPage(_config.getErrorPages("403"));
			return (403);
		}
		buffer << file.rdbuf();
		_response = buffer.str();
		file.close();
	}
	else
	{
		_response = readErrorPage(_config.getErrorPages("404"));
		return (404);
	}
	return (200);
}

std::string	Response::readErrorPage(const std::string & path)
{
	std::ifstream	file;
	std::stringstream	buffer;

	if (fileExist(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (!file.is_open())
			return ("<!DOCTYPE html>\n<html><title>404</title><body> Cant open error page !</body></html>\n");
		buffer << file.rdbuf();
		file.close();
		_contentType = "text/html";
		return (buffer.str());
	}
	return ("<!DOCTYPE html>\n<html><title>404</title><body>Error page does not exist </body></html>\n");
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
std::string	Response::getHeader(size_t size, std::string path)
{
	std::string	header;

	setContentLength(size);
	if (path != "")
		setContentType(path);
	header = writeHeader();
	return (header);
}

std::string	Response::writeHeader(void)
{
	initStatusMsg();
	std::string	header = "HTTP/1.1 " + to_string(_code) + " " + getStatuMsg() + "\r\n";

	if (!_contentLength.empty())
		header += "Content-Length: " + _contentLength + "\r\n";
	if (!_contentType.empty())
		header += "Content-Type: " + _contentType + "\r\n";
	return (header);
}

void	Response::initStatusMsg()
{
	_statusMsg[100] = "Continue";
	_statusMsg[200] = "ok";
	_statusMsg[204] = "No Content";
	_statusMsg[400] = "Bad Request";
	_statusMsg[403] = "Forbidden";
	_statusMsg[404] = "Not Found";
	_statusMsg[500] = "Internal Server Error";
}

std::string	Response::getStatuMsg()
{
	if (_statusMsg.find(_code) == _statusMsg.end())
		return ("Unknown Code");
	return (_statusMsg[_code]);
}

void	Response::setCode(int code) { _code = code; }

void	Response::setContentLength(size_t size) { _contentLength = to_string(size); }

void	Response::setContentType(std::string path)
{
	if (_contentType != "")
		return ;
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

void	Response::setRequest(Request &request) {_request = request;}
std::string	Response::getResponse() {return _response;}
