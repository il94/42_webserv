/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:50:45 by auzun             #+#    #+#             */
/*   Updated: 2023/04/01 06:07:22 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(void): _reqBody(""){};

void	Request::setRequestAtr(std::string req)
{
	std::string	firstReqLine(req, 0, req.find("\n"));
	std::string	tmp;
	std::stringstream	ss(firstReqLine);

	while (ss >> tmp)
	{
		_data.push_back(tmp);
	}
	if(_data.size() != 3 || _data[2].find("HTTP/") == std::string::npos)
		return ;/*send exception*/
	if (_data[2].find("HTTP/1.1") == std::string::npos)
		return ; /*send exception de http version is invalid*/

	std::istringstream	stream(req);
	std::string			tmp;
	while (std::getline(stream, tmp))
	{
		if (tmp == "")
			break;
		size_t	boundary = tmp.find(":");
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

void	Request::setQueryM()
{
	std::string	queryString;

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
		queryString = _reqBody;
	else
		return ;
	_requestContent = queryString;
	while (queryString != "")
	{
		std::string	subQuery = queryString.substr(0, queryString.find("&"));
		_queryM[subQuery.substr(0, subQuery.find("=") + 1)] = subQuery.substr(subQuery.find("=") + 1);
		if (queryString.find("&") != std::string::npos)
			queryString.substr(queryString.find("&") + 1);
		else
			queryString = "";
	}
}

std::string Request::getURL() const { return _data[1]; }

std::string Request::getMethod() const { return _data[0]; }

std::string Request::getRequestContent() const { return _requestContent; }

std::string Request::getHTTPVersion() const { return _data[2]; }

std::string Request::getElInHeader(std::string key) const { return _headerM[key]; }

std::string Request::getRequestBody() const { return _reqBody; }

std::string Request::getRequestBody() const { return _queryM; }
