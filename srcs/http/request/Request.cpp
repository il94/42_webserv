/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:50:45 by auzun             #+#    #+#             */
/*   Updated: 2023/03/31 23:08:24 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(void){};

Request::Request(std::string req): _request(req){};

Request::Request(char * req): _request(req){};

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
