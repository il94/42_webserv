/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:50:55 by auzun             #+#    #+#             */
/*   Updated: 2023/03/31 23:06:15 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <map>
# include <string>
# include <map>
# include <vector>
# include <sstream>
# include <cstddef>

class	Request
{
	public:
		Request(std::string req);
		Request(char * req);
		~Request(void);

		void	setRequestAtr(std::string req);

	private:
		Request(void);

		std::vector<std::string>	_data;
		std::map<std::string, std::string>	_headerM;

		std::string	_reqBody;
		std::string	_request;
		std::string	_method;
	
};

#endif