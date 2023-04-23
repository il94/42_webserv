/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:50:45 by auzun             #+#    #+#             */
/*   Updated: 2023/04/23 17:13:21 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/*=============================== Constructors ===============================*/
Request::Request(void): _requestContent(""), _reqBody(""),  _ret(200){}

Request::~Request(void){}

/*================================= Methods ==================================*/

bool	Request::badFirstLine()
{
	_ret = 400;
	if(_data.size() != 3 || _data[2].find("HTTP/") == std::string::npos)
	{
		std::cerr << RED << "Error : the request cannot be processed, somes elements are missing" << END << std::endl;
		return true;
	}
	if (_data[2] != "HTTP/1.0" && _data[2] != "HTTP/1.1")
	{
		std::cerr << RED << "Error : the request cannot be processed, BAD HTTP VERSION" << END << std::endl;
		return true;
	}
	if (_data[0] != "POST"  && _data[0] != "DELETE" && _data[0] != "GET")
	{
		std::cerr << RED << "Error : the request cannot be processed, INVALID METHOD" << END << std::endl;
		return true;
	}
	_ret = 200;
	return false;
}

std::vector<std::string>	Request::splitURL()
{
	std::vector<std::string>	splitedURL;
	std::string	URL = 			getURL();

	URL = URL[URL.size() - 1]  != '/' ? URL + "/" : URL;

	do
	{
		splitedURL.push_back(URL);
		URL = URL.substr(0, rfind(URL, "/"));
	} while (URL != "");
	return (splitedURL);
}

void	Request::parseHeader(const std::string & req)
{
	std::string	firstReqLine(req, 0, req.find("\n"));
	std::string	tmp;
	std::stringstream	ss(firstReqLine);

	while (ss >> tmp)
		_data.push_back(tmp);

	if (badFirstLine())
		return ;

	if (_data[1][0] == '/')
		_data[1] = _data[1].substr(_data[1].find("/") + 1, _data[1].size() - 1);

	std::cout << _data[1] << std::endl;
	std::istringstream	stream(req);
	size_t	boundary = std::string::npos;
	while (std::getline(stream, tmp))
	{
		if (tmp == "")
			break;
		boundary = tmp.find(":");
		if (boundary != std::string::npos)
		{
			std::string	key(tmp, 0, boundary);
			std::string	value(tmp, boundary + 2);
			_headerM[key] = value;
		}
	}
	size_t	bodyPosition = req.find("\r\n\r\n");
	if (bodyPosition != std::string::npos)
		_reqBody = req.substr(bodyPosition + 2);
}

void	Request::parseBody()
{
	std::string	queryString = "";

	_requestContent = "";
	if (getMethod() == "GET")
	{
		std::string	URL = getURL();
		size_t	queryPos = URL.find("?");
		if (queryPos == std::string::npos)
			return;
		_data[1] = URL.substr(0, queryPos);
		queryString = URL.substr(queryPos + 1);
	}
	else if (getMethod() == "POST")
	{
		queryString = _reqBody;
		queryString = queryString.substr(2 , queryString.size() - 1);
	}
	else
		return ;
	_requestContent = queryString;
	while (queryString != "")
	{
		std::string	subQuery = queryString.substr(0, queryString.find("&"));
		_queryM[subQuery.substr(0, subQuery.find("="))] = subQuery.substr(subQuery.find("=") + 1);
		if (queryString.find("&") != std::string::npos)
			queryString = queryString.substr(queryString.find("&") + 1);
		else
			queryString = "";
	}
}

/*================================ Accessors =================================*/

std::string Request::getURL() const { return _data[1]; }

std::string Request::getMethod() const { return _data[0]; }

std::string Request::getRequestContent() const { return _requestContent; }

std::string Request::getHTTPVersion() const { return _data[2]; }

std::string Request::getElInHeader(const std::string & key) { return _headerM[key]; }

std::map<std::string, std::string>	Request::getHeaderM() { return _headerM; }

std::string Request::getRequestBody() const { return _reqBody; }

int	Request::getRet() const {return _ret; }
