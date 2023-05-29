/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:27 by auzun             #+#    #+#             */
/*   Updated: 2023/05/28 23:47:15 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

		// Constructors/Destructor

Response::Response(void)
{
	
	_port = 8000;
	_host = "127.0.0.1";

	_path = "";

	/*Upload*/
	_uploadStatu = EMPTY;
	_boundary = "";
	_uploadPath = "./";
	_uploadFileName = "";
	_controler = "";
	_content = NULL;
	/*=====*/

	/*Header*/
	_contentLength = "0"; 
	_contentType = "";
	_code = 200;
	/*=====*/

	_response = "";
}

Response::Response(Request & request, const Config & config, int port, std::string host)
{
	_port = port;
	_host = host;

	_path = "";

	/*Upload*/
	_uploadStatu = EMPTY;
	_boundary = "";
	_uploadPath = "./";
	_uploadFileName = "";
	_controler = "";
	_content = NULL;
	/*=====*/

	/*Header*/
	_contentLength = "0"; 
	_contentType = "";
	_code = request.getRet();
	/*=====*/

	_request = request;
	_config = config;

	_response = "";

	if (_code != 400)
		_location = findLocation();
}

Response::Response(const Response & src)
{
	_port = src._port;
	_host = src._host;

	_path = src._path;

	/*Upload*/
	_uploadStatu = src._uploadStatu;
	_boundary = src._boundary;
	_uploadPath = src._uploadPath;
	_uploadFileName = src._uploadFileName;
	_controler = src._controler;
	_files = src._files;
	_headerName = src._headerName;
	_content = src._content;
	/*=====*/

	/*Header*/
	_contentLength = src._contentLength;
	_contentType = src._contentType;
	_statusMsg = src._statusMsg;
	_code = src._code;
	_cookies = src._cookies;
	/*=====*/

	_request = src._request;
	_config = src._config;
	_location = src._location;
	_response = src._response;
}


Response::~Response(void) {}


		// Assignment operator

Response &	Response::operator=(const Response & src)
{
	_port = src._port;
	_host = src._host;

	_path = src._path;

	/*Upload*/
	_uploadStatu = src._uploadStatu;
	_boundary = src._boundary;
	_uploadPath = src._uploadPath;
	_uploadFileName = src._uploadFileName;
	_controler = src._controler;
	_files = src._files;
	_headerName = src._headerName;
	_content = src._content;
	/*=====*/

	/*Header*/
	_contentLength = src._contentLength;
	_contentType = src._contentType;
	_statusMsg = src._statusMsg;
	_code = src._code;
	_cookies = src._cookies;
	/*=====*/

	_request = src._request;
	_config = src._config;
	_location = src._location;
	_response = src._response;
	return (*this);
}

		// Methods

void	Response::generate()
{
	if (_uploadStatu == READ)
	{
		POST();
	}
	else
	{
		if (_code == 400)
		{
			_response = readErrorPage(_config.getErrorPages(to_string(_code)));
			_response = generateHeader(_response.size(), "") + "\r\n" + _response;
			return ;
		}
		setPath();
		
		std::vector<std::string> tmpAllowedMethods = _location.getAllowedMethods();
		
		if (std::find(tmpAllowedMethods.begin(), tmpAllowedMethods.end(), _request.getMethod())\
			== tmpAllowedMethods.end())
				_code = 405;

		unsigned long	bodySize = 0;
		std::map<std::string, std::string> header = _request.getHeaderM();
		if (header.find("Content-Length") != header.end())
		{
			bodySize = atol((header["Content-Length"]).c_str());
			if (_config.getMaxBodySize() < bodySize)
				_code = 413;
		}
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
}

extern bool running;
void	Response::GET(void)
{
	if (findCookie() == true)
		extractCookie();
	else
		std::remove("cookies.txt");

	if (findCGI() == true and _code != 500)
	{
		CGI cgi(_request);
		_response = cgi.execCGI(_path);
		if (running == false)
			return ;
		while (!_response.empty() && (_response[0] == '\n' || _response[0] == '\r'))
			_response.erase(0, 1);
			
		size_t	bodyPosition = _response.find("\r\n\r\n");
		if (bodyPosition == std::string::npos)
			_code = 500;
		else
		{
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
					else if (key == "Set-Cookie")
						_cookies.push_back(value);
				}
			}

			_response = _response.substr(bodyPosition + 4);
			_response = generateHeader(_response.size(), "") + "\r\n" + _response;
			return ;
		}
	}
	if (_code == 200)
		_code = readContent();
	else
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = generateHeader(_response.size(), _path) + "\r\n" + _response;
}

void	Response::POST(void)
{
	if (findCookie() == true)
		extractCookie();
	else
		std::remove("cookies.txt");

	if (_code != 500 && (_uploadStatu == READ || findCGI() == true))
	{
		/*==========================================================*/
		upload();
		if (_uploadStatu == READ)
			return ;
		/*==========================================================*/
		if (_code == 200)
		{
			CGI cgi(_request);
			_response = cgi.execCGI(_path);
			if (running == false)
				return ;
			while (!_response.empty() && (_response[0] == '\n' || _response[0] == '\r'))
				_response.erase(0, 1);
			size_t	bodyPosition = _response.find("\r\n\r\n");

			if (bodyPosition == std::string::npos)
				_code = 500;
			else
			{
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
						else if (key == "Set-Cookie")
							_cookies.push_back(value);
					}
				}
				_response = _response.substr(bodyPosition + 4);

			}
		}
	}
	else if (_code == 200)
	{
		_code = 204;
		_response = "";
	}
	if (_code != 200 && _code != 204)
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = generateHeader(_response.size(), "") + "\r\n" + _response;
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

		// Configuration methods

void	Response::setRequest(Request &request) { _request = request; }

void	Response::setConfig(Config &config) { _config = config; }

void	Response::setContent(std::vector<unsigned char> * vec) { _content = vec; }

void	Response::setPath()
{
	std::string	root = _location.getRoot();

	std::string	locationPath = _location.getPath();

	_path = root.substr(0, root.size() - 1) + _request.getURL();

	if (root != locationPath)
		_path.erase(_path.find(locationPath.substr(0, locationPath.size() - 1)), locationPath.size() - 1);
	if (isDir(_path))
	{
		std::vector<std::string>					indexs= _location.getIndex();
		std::vector<std::string>::const_iterator	it = indexs.begin();

		while (it != indexs.end())
		{
			if (isFile(*it) and it->find(_path) != std::string::npos)
			{
				_path = *it;
				return ;
			}
			it++;
		}
	}
}

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

void	Response::setCode(int code) { _code = code; }

void	Response::setContentLength(size_t size) { _contentLength = to_string(size); }



		// Data access methods

std::string	Response::getStatuMsg()
{
	if (_statusMsg.find(_code) == _statusMsg.end())
		return ("Unknown Code");
	return (_statusMsg[_code]);
}

int	Response::getUploadStatu() { return _uploadStatu ;}

std::string	Response::getUploadFileName() {return _uploadFileName;}

std::string	Response::getUploadPath() {return _location.getUploadPath();}

std::string	Response::getResponse() {return _response;}


		// Response utility methods

int	Response::readContent(void)
{
	std::ifstream	file;
	std::stringstream	buffer;

	_response = "";	
	if (isFile(_path))
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
	else if (isDir(_path) == true)
	{
		_response = readErrorPage(_config.getErrorPages("422"));
		return (422);
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
			return (readErrorPage(_config.getErrorPages("500")));
		buffer << file.rdbuf();
		file.close();
		_contentType = "text/html";
		return (buffer.str());
	}
	else if (path == "default_files/404.html")
	{
		_contentType = "text/plain";
		return ("Error: default error pages corrupted.");
	}
	return (readErrorPage(_config.getErrorPages("default_404")));
}

void	Response::extractCookie()
{
	std::remove("cookies.txt");

	std::ofstream	file("cookies.txt");
	if (file.is_open() == false)
	{
		_code = 500;
		return ;
	}

	std::string											header = _request.getHeaderM()["Cookie"];
	std::vector<std::pair<std::string,std::string> >	cookies;
	cookies = splitCookieHeader(header);

	for (std::vector<std::pair<std::string,std::string> >::iterator it = cookies.begin();
		it != cookies.end(); it++)
	{
		file << it->first << '\n';
		file << it->second << '\n';
		file << '\n';
	}	
	
	file.close();
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


std::vector<std::pair<std::string,std::string> >	Response::splitCookieHeader(const std::string &src)
{
	std::vector<std::pair<std::string,std::string> >	cookies;
	std::pair<std::string,std::string>					pair;
	std::string											element;
	
	for (std::string::const_iterator it = src.begin(); it != src.end(); it++)
	{
		if (*it == '=')
		{
			pair.first = element;
			element.clear();
		}
		else if (it + 1 == src.end() or *it == ';')
		{
			pair.second = element;		
			cookies.push_back(pair);
			if (*it == ';')
				it++;
			element.clear();
		}
		else
			element.push_back(*it);
	}
	return (cookies);
}

bool		Response::findCGI()
{

	std::string	url = _request.getURL();
	if (url[url.size() - 1] == '/' or rfind(url, ".") == static_cast< size_t >( -1 ))
		return false;

	std::vector<std::string>	allowedCGI = _location.getAllowedCGI();
	std::string		urlFileExtension = url.substr(rfind(url, "."));

	if (urlFileExtension != ".py" and urlFileExtension != ".php")
		return (false);
	
	if (std::find(allowedCGI.begin(), allowedCGI.end(), urlFileExtension)\
		== allowedCGI.end())
	{
		_code = 403;
		return false;
	}

	std::vector<std::string>					cgiPaths = _location.getCGIBin();
	std::vector<std::string>::const_iterator	it = cgiPaths.begin();
	std::vector<std::string>					splitedURL = _request.splitURL();
	std::vector<std::string>::const_iterator	yt;
	std::string									newPath = "";

	while (it != cgiPaths.end())
	{
		yt = splitedURL.begin();
		while (yt != splitedURL.end())
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
	_code = 404;
	return false;
}

bool		Response::findCookie()
{
	std::map<std::string, std::string>	header = _request.getHeaderM();
	return (header.find("Cookie") != header.end());
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
	return _config.getDefaultLocation();
}

		// Header management methods

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

	for (std::vector<std::string>::iterator	it = _cookies.begin();
			it != _cookies.end(); it++)
		header += "Set-Cookie: " + *it + "\r\n";

		
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
	_statusMsg[422] = "Unprocessable Entity";
	_statusMsg[500] = "Internal Server Error";
	_statusMsg[504] = "Gateway Timeout";
}
