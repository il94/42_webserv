/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 04:15:58 by auzun             #+#    #+#             */
/*   Updated: 2023/04/24 15:57:06 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "../srcs/utils/utils.hpp"
//... add basic includes..

# define RED "\033[31m"
# define PURPLE "\033[35m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define END "\033[0m"

typedef enum e_flag
{
	EMPTY	= 0,
	ERROR	= 1 << 0,
	CONTENT	= 1 << 1,
    READ	= 1 << 2,
	STOP	= 1 << 3
}			t_flag;

#endif