/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:44:27 by auzun             #+#    #+#             */
/*   Updated: 2023/05/19 00:05:57 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*=============================== Constructors ===============================*/
Response::Response(void): _port(8000), _host("127.0.0.1"),
	_path(""), _boundary(""), _uploadFileName(""), _contentLength("0"), _contentType(""),
		_code(200), _response("") {}

Response::Response(Request & request, Config & config,
	int port, std::string host) : _port(port), _host(host), _path(""), _boundary(""),
	_uploadFileName(""), _contentLength(""), _contentType(""), _code(request.getRet()),
		_request(request), _config(config), _response("")
{
	if (_code != 400)
		_location = findLocation();
}

Response::~Response(void) {}


/*================================= Methods ==================================*/

void	Response::generate()
{
	if (_uploadStatu == WAITING)
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
}

/*============================= HTTP Methods =================================*/

void	Response::GET(void)
{
	if (findCGI() == true)
	{
		CGI cgi(_request);
		_response = cgi.execCGI("./html/cgi_test/" + _request.getURL());
		std::cout << YELLOW << _response << END << std::endl;
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
		_response = generateHeader(_response.size(), "") + _response;
		return ;
	}
	if (_code == 200)
		_code = readContent();
	else
		_response = readErrorPage(_config.getErrorPages(to_string(_code)));
	_response = generateHeader(_response.size(), _path) + "\r\n" + _response;
}

void	Response::POST(void)
{
	if (findCGI() == true)
	{
		/*==========================================================*/
		upload();
		if (_uploadStatu == WAITING)
			return ;
		/*==========================================================*/
		if (_code == 200)
		{
			CGI cgi(_request);
			cgi.setUploadInfo(_uploadFileName, _location.getUploadPath());
			_response = cgi.execCGI(_path);
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
	}
	else if (_code != 403)
	{
		_code = 204;
		_response = "";
	}
	if (_code != 200 || _code != 203)
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
	return (readErrorPage(_config.getErrorPages("default_404")));
}


std::string	Response::getMPFD_Header()
{
	/*get the header of multi part form data*/
	std::vector<unsigned char>::iterator it = _content.begin();

	while (it != _content.end() && _controler.find("\r\n\r\n") == std::string::npos)
	{
		_size++;
		_controler += *it;
		it = _content.erase(it);
	}
	return (_controler);
}

void	Response::upload()
{
	/*if _uploadStatu == START set the boundary, uploadPath and controler to not redefine them next time*/
	if (_uploadStatu == START)
	{
		std::string	Content_type = _request.getElInHeader("Content-Type");
		if (Content_type.find("multipart/form-data"))
		{
			size_t	boundaryPos = Content_type.find("boundary=");
			if (boundaryPos != std::string::npos)
				_boundary = Content_type.substr(Content_type.find("boundary=" + 9));
			else
				return ;
			_uploadPath = _location.getUploadPath();
			_controler = "";
		}
		else
			return ;
	}
	/*================================================================================================*/

	/*know we need to open the file on which we gonna write the _content */
	std::ofstream	outfile;

	    /*if the _uploadFileName is already set we just open it*/
	if (_uploadFileName != "")
	{ 
		outfile.open(_uploadPath + _uploadFileName, std::ios_base::app);
		if (outfile.is_open() == false)
		{
			_code = 400;
			_uploadStatu = STOP;
			return ;
		}
	}
	    /*===================================================*/
		/*otherwise it means that we haven't read the form header yet */
	else
	{
		std::string header = getMPFD_Header(); // take the header
		if (header.find("\r\n\r\n") == std::string::npos) // if we dont have "\r\n\r\n" 
		{												 //it means that we have not read all the content of the header 
			_uploadStatu = WAITING;						// so we put the _uploadStatu to WAITING 
			return ;								   // to tell the server that we are still waiting for data
		}
		/*get the name of the file in the header*/
		size_t				filenameStartPos = header.find("filename=\"") + 10;
		size_t				filenameEndPos = header.find("\"", filenameStartPos);
		if (filenameStartPos != std::string::npos && filenameEndPos != std::string::npos
			&& filenameEndPos > filenameStartPos)
			_uploadFileName = header.substr(filenameStartPos, filenameEndPos - filenameStartPos);
		else /*if there is not "filename=" in the header it means that an other type of data was send with mulit part form data so error*/
		{
			_code = 400;
			_uploadStatu = STOP;
			return ;
		}
		/*===================================*/
		outfile.open(_uploadPath + _uploadFileName);
		if (outfile.is_open() == false)
		{
			_code = 400;
			_uploadStatu = STOP;
			return ;
		}
		_controler = "";
	}
	    /*=============================================================*/
	/*===================================================================*/

	/*setup the _controler which will take boundary.size() ahead of _content to check if we reach the end of a form*/
	std::vector<unsigned char>::iterator	it = _content.begin();
	if (_controler.size() < (_boundary.size() + 2))
	{
		while (it != _content.end() && _controler.size() < (_boundary.size() + 2))
		{
			_controler += *it;
			it++;
		}
	}
	/*===========================================================================================================*/
	std::vector<unsigned char>::iterator	yt = _content.begin();
	/*in this loop while checking if we arrive at the end of _content or at the end of the all form 
	or of a form we will increment size which will allow us to have the complete size of the body, 
	we will also update the control while removing its first element then adding the character
	 *it in the file in question*/
	while (yt != _content.end() && it != _content.end() && (_controler.find(_boundary + "\r\n") == std::string::npos 
		&& _controler.find(_boundary + "--") == std::string::npos))
	{
		_size++;
		_controler += *it;//
		_controler.erase(0, 1);//
		outfile << *yt;
		yt = _content.erase(yt);
	}
	outfile.close();
	/*_boundary + -- means we have completed all forms so we can stop the upload process*/
	if (_controler.find(_boundary + "--") != std::string::npos)
	{
		_uploadStatu = STOP;
		return ;
	}
	/*_boundary + \r\n means we have completed an form so we reset _uploadFilname and _cotroler*/
	else if (_controler.find(_boundary + "\r\n") != std::string::npos)
	{
		_uploadFileName = "";
		_controler = "";
	}
	/*if we reach the end of _content we set the status to waiting */
	if (it == _content.end())
	{
		_uploadStatu = WAITING;
		return ;
	}
	/*it means that we can continue the download process so we call upload() recursively*/
	upload();
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
	if (url[url.size() - 1] == '/' or rfind(url, ".") == static_cast< size_t >( -1 ))
		return false;

	std::vector<std::string>	allowedCGI = _location.getAllowedCGI();
	
	std::string		urlFileExtension = url.substr(rfind(url, "."));

	// std::cout << GREEN << urlFileExtension << END << std::endl;

	if (urlFileExtension != ".py" and urlFileExtension != ".php" and urlFileExtension != ".sh")
		return (false);
	
	if (std::find(allowedCGI.begin(), allowedCGI.end(), urlFileExtension)\
		== allowedCGI.end())
	{
		_code = 403;
		return false;
	}

	// std::cout << GREEN << urlFileExtension << END << std::endl;

	std::vector<std::string>					cgiPaths = _location.getCGIBin();
	std::vector<std::string>::const_iterator	it = cgiPaths.begin();

	std::vector<std::string>					splitedURL = _request.splitURL();
	std::vector<std::string>::const_iterator	yt;

	std::string									newPath;
	

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

	// need to verif / location
	return _config.getDefaultLocation();
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
	_statusMsg[422] = "Unprocessable Entity";
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

void	Response::setContent(std::vector<unsigned char> & vec)
{
	_content = vec;
}


std::string	Response::getUploadFileName() {return _uploadFileName;}

std::string	Response::getUploadPath() {return _location.getUploadPath();}

std::string	Response::getResponse() {return _response;}
