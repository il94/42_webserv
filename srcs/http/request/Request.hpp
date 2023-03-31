/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:50:55 by auzun             #+#    #+#             */
/*   Updated: 2023/03/31 13:45:39 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REQUEST_HPP
# define REQUEST_HPP

#include <iostream>


class	Request
{
	public:
		Request(std::string req);
		Request(char * req);
		~Request(void);

	private:
		Request(void);

		std::string	_request;
		std::string	_method;
		
	
};

#endif