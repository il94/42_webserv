/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:08:39 by auzun             #+#    #+#             */
/*   Updated: 2023/04/23 03:58:19 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>

#include <algorithm>
size_t			rfind(const std::string& str, const std::string& substr);
std::string								to_string(size_t n);
std::vector<std::string>				fileToVector( std::string path );
std::vector<std::vector <std::string> > splitFileConfig(std::string path);

template <typename T>
void	displayString(T str)
{
	std::cout << str << std::endl;
}

template <typename T>
void	displayVector(T vec)
{
	std::cout << "===========================" << std::endl;
	std::cout << std::endl;
	std::for_each(vec.begin(), vec.end(), displayString<std::string>);
	std::cout << std::endl;

}

#endif