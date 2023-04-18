/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:50:55 by auzun             #+#    #+#             */
/*   Updated: 2023/04/18 05:46:40 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REQUEST_HPP
# define REQUEST_HPP

# include "../../../include/webserv.hpp"
# include <map>
# include <string>
# include <map>
# include <vector>
# include <sstream>
# include <cstddef>

class	Request
{
	public:
		Request(void);
		~Request(void);

		std::string	getMethod() const;
		std::string	getURL() const;
		std::string	getRequestContent() const;
		std::string	getHTTPVersion() const;
		std::string	getElInHeader(const std::string & key);
		std::map<std::string, std::string>	getHeaderM();
		std::string	getRequestBody() const;
		std::map<std::string, std::string>	getQueryMap() const;

		bool	badFirstLine();

		void	setRequestAtr(const std::string & req);
		void	setQueryM();
	
	private:
		std::vector<std::string>	_data;
		std::map<std::string, std::string>	_headerM;
		std::map<std::string, std::string>	_queryM;

		std::string	_requestContent;
		std::string	_reqBody;
		int			_ret;

};

#endif