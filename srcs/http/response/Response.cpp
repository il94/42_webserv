/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:27 by auzun             #+#    #+#             */
/*   Updated: 2023/05/10 13:07:19 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*=============================== Constructors ===============================*/
Response::Response(void): _port(8000), _host("127.0.0.1"),
	_path(""), _uploadFileName(""), _contentLength("0"), _contentType(""),
		_code(200), _response("") {}

Response::Response(Request & request, Config & config,
	int port, std::string host) : _port(port), _host(host), _path(""),
	_uploadFileName(""), _contentLength(""), _contentType(""), _code(request.getRet()),
		_request(request), _config(config), _response(""),
			_location(findLocation()){}

Response::~Response(void) {}


/*================================= Methods ==================================*/

void	Response::generate()
{
	if (_code == 400)
	{
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
		_response = generateHeader(_response.size(), "") + "\r\n" + _response;
		return ;
	}
	setPath();
	std::cout << YELLOW << _path << END << std::endl;
	_location.display();

	if (std::find(_location.getAllowedMethods().begin(), _location.getAllowedMethods().end(), _request.getMethod())\
		== _location.getAllowedMethods().end())
			_code = 405;
	else if (_config.getMaxBodySize() < _request.getRequestBody().size())
		_code = 413;
	if (_code == 405 || _code == 413)
	{
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
		_response = generateHeader(_response.size(), "") + "\r\n" + _response;
		return ;
	}
	else if (_location.getRedirection().first / 100 == 3)
	{
		_code = _location.getRedirection().first;
		_response = generateHeader(_response.size(), "") + "\r\n" + _response;
		return ;
	}
	if (_request.getMethod() == "GET")
		GET();
	else if (_request.getMethod() == "POST")
		POST();
	else
		DELETE();
}

/*============================= HTTP Methods =================================*/

void	Response::GET(void)
{
	std::cout << _path << std::endl;
	// if (fileExist("./html/cgi_test/" + _request.getURL()))
	// {
	// 	CGI cgi(_request);
	// 	_response = cgi.execCGI("./html/cgi_test/" + _request.getURL());
	// 	std::cout << YELLOW << _response << END << std::endl;
	// 	while (!_response.empty() && (_response[0] == '\n' || _response[0] == '\r'))
	// 		_response.erase(0, 1);
	// 	size_t	bodyPosition = _response.find("\r\n\r\n");
	// 	size_t	boundary = std::string::npos;

	// 	std::string			tmp;
	// 	std::istringstream	stream(_response);
		
	// 	while (std::getline(stream, tmp))
	// 	{
	// 		if (tmp == "")
	// 			break;
	// 		boundary = tmp.find(":");
	// 		if (boundary != std::string::npos)
	// 		{
	// 			if (boundary > bodyPosition)
	// 				break;
	// 			std::string	key(tmp, 0, boundary);
	// 			std::string	value(tmp, boundary + 2);
	// 			if (key == "Status")
	// 				_code = std::atoi(value.c_str());
	// 			else if (key == "Content-Type")
	// 				_contentType = value;
	// 		}
	// 	}
	// 	_response = _response.substr(bodyPosition + 2);
	// 	_response = generateHeader(_response.size(), "") + _response;
	// 	return ;
	// }
	if (_code == 200)
		_code = readContent();
	else
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = generateHeader(_response.size(), _path) + "\r\n" + _response;
}

void	Response::POST(void)
{
	std::cerr << RED << "./html/cgi_test" + _request.getURL() << END << std::endl;
	if (fileExist("./html/cgi_test/" + _request.getURL()))
	{
		updateContentIfBoundary();
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
	if (_code == 500)
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = generateHeader(_response.size(), "") + _response;
}

void	Response::DELETE(void)
{
	if (fileExist(_path))
	{
		if (remove(_path.c_str()) == 0)
			_code = 204;
		else
			_code = 403;
	}
	else
		_code = 404;
	if (_code == 404 || _code == 403)
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = generateHeader(_response.size(), _path) + "\r\n" + _response;
}

/*============================= UTILS =================================*/

int	Response::readContent(void)
{
	std::ifstream	file;
	std::stringstream	buffer;

	_response = "";
	if (fileExist(_path))
	{
		file.open(_path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
		{
			_response = readErrorPage(_config.getErrorPages("403"));
			return (403);
		}
		buffer << file.rdbuf();
		_response = buffer.str();
		file.close();
	}
	else if (_location.getListing() == true && isDir(_path) == true)
	{
		_response = generateAutoIndex();
		_contentType = "text/html";
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


void	Response::updateContentIfBoundary()
{
	const std::string	body = _request.getRequestBody();
	size_t				boundary = body.find("------WebKitFormBoundary");
	if (boundary == std::string::npos)
		return ;
	std::string			requestContent = body.substr(boundary);

	size_t				filenameStartPos = requestContent.find("filename=\"") + 10;
	size_t				filenameEndPos = requestContent.find("\"", filenameStartPos);
	if (filenameStartPos != std::string::npos && filenameEndPos != std::string::npos
		&& filenameEndPos > filenameStartPos)
		_uploadFileName = requestContent.substr(filenameStartPos, filenameEndPos - filenameStartPos);
	
	requestContent = requestContent.substr(requestContent.find("\r\n\r\n") + 4);
	requestContent = requestContent.substr(0, requestContent.find("------WebKitFormBoundary") - 4);
	_request.setRequestContent(requestContent);
}


int	Response::fileExist(std::string path)
{
	struct stat	stats;

	if (stat(path.c_str(), &stats) == 0)
			return 1;
	return 0;
}

int	Response::isFile(std::string path)
{
	struct stat	stats;

	if (stat(path.c_str(), &stats) == 0)
	{
		if (S_ISREG(stats.st_mode))
			return 1;
	}
	return 0;
}

int	Response::isDir(std::string path)
{
	struct stat	stats;

	if (stat(path.c_str(), &stats) == 0)
	{
		if (S_ISDIR(stats.st_mode))
			return 1;
	}
	return 0;
}

bool		Response::findCGI()
{
	std::string	url = _request.getURL();

	if (url[url.size() - 1] == '/')
		return false;

	std::vector<std::string>	allowedCGI = _location.getAllowedCGI();
	std::string					urlFileName = url.substr(rfind(url, "/"));
	std::string					urlFileExtension = urlFileName.substr(rfind(url, "."));

	if (std::find(allowedCGI.begin(), allowedCGI.end(), urlFileExtension)\
		== allowedCGI.end())
		return false;

	std::vector<std::string>					cgiPaths = _location.getCgi();
	std::vector<std::string>::const_iterator	it = cgiPaths.begin();

	std::vector<std::string>					splitedURL = _request.splitURL();
	std::vector<std::string>::const_iterator	yt;

	std::string									newPath;

	while (it != cgiPaths.end())
	{
		yt = splitedURL.begin();
		while (yt != cgiPaths.end())
		{
			if ((*it).find(*yt) != std::string::npos)
			{
				newPath = *it;
				newPath.erase(newPath.find(*yt), newPath.size() - newPath.find(*yt));
				newPath = newPath[newPath.size() - 1] != '/' ? newPath + "/": newPath;
				newPath += url;
				if (isFile(newPath))
				{
					_path = newPath;
					return true;
				}
			}
			yt++;
		}
		it++;
	}
	return false;
}

Location	Response::findLocation()
{
	std::map<std::string, Location>				locationM = _config.getLocations();
	std::vector<std::string>					splitedURL = _request.splitURL();
	std::vector<std::string>::const_iterator	it = splitedURL.begin();

	while (it != splitedURL.end())
	{
		if (locationM.find(*it) != locationM.end())
		{
			return locationM[*it];

		} else if (locationM.find((*it).substr(0, rfind(*it, "/"))) \
			!= locationM.end()){
			return locationM[(*it).substr(0, rfind(*it, "/"))];
		} else if (locationM.find((*it) + "/") \
			!= locationM.end()){
			return locationM[(*it) + "/"];
		}
		it++;
	}


	return locationM["/"];
}

/*============================= HTTP HEADER =================================*/

std::string	Response::generateHeader(size_t size, std::string path)
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
	std::string	header = "";

	initStatusMsg();

	if (_code == 405)
		header = "HTTP/1.1 405 Method Not Allowed\r\n";
	else if (_code == 413)
		header = "HTTP/1.1 413 Payload Too Large\r\n";
	else if (_code / 100 == 3)
	{
		header = "HTTP/1.1 " + to_string(_code) + " " + getStatuMsg() + "\r\n";
		header += "Location: " + _location.getRedirection().second + "\r\n";
	}
	else
		header = "HTTP/1.1 " + to_string(_code) + " " + getStatuMsg() + "\r\n";

	if (_host != "")
		header += "Server: " + _host + "\r\n";
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
	_statusMsg[302] = "Found";
	_statusMsg[303] = "Moved Permanently";
	_statusMsg[400] = "Bad Request";
	_statusMsg[403] = "Forbidden";
	_statusMsg[404] = "Not Found";
	_statusMsg[405] = "Method Not Allowed";
	_statusMsg[500] = "Internal Server Error";
}

/*================================ Accessors =================================*/

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
	std::string	type = path.substr(path.rfind(".") + 1);
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

void	Response::setRequest(Request &request)
{
	_code = request.getRet();
	_request = request;
}

void	Response::setConfig(Config &config)
{
	_config = config;
	_location = findLocation();
}

void	Response::setPath()
{
	std::string	root = _location.getRoot();
	
	root = root[root.length() - 1] == '/' ? root : root + "/";

	std::string	locationPath = _location.getPath();

	locationPath = locationPath[locationPath.length() - 1] == '/' ?\
		locationPath : locationPath + "/";
		
	_path = root.substr(0, root.size() - 1) + _request.getURL();
	
	_path.erase(_path.find(locationPath), locationPath.size());
	if (isDir(_path))
	{
		_path = _path[_path.length() - 1] == '/' ? _path : _path + "/";
		_path= _path + _location.getIndex()[0];
	}
}

std::string	Response::getResponse() {return _response;}
