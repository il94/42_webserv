/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:08:39 by auzun             #+#    #+#             */
/*   Updated: 2023/04/22 19:46:09 by ilandols         ###   ########.fr       */
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

std::string								to_string( size_t );
std::vector<std::string>				fileToVector( std::string );
std::vector<std::vector <std::string> > splitFileConfig( std::string );

std::string					findInFileContent(const std::vector<std::string> &, const std::string &);
std::vector<std::string>	multipleFindInFileContent(const std::vector<std::string> &, const std::string &);

template <typename T>
void	displayElement(const T &element, const std::string &name)
{
	std::cout << name + " = " << element << std::endl;
}

template <typename T>
void	displayPair(const T &pair, const std::string &name)
{
	std::cout << name + " " << pair.first << " = " << pair.second << std::endl;
}

template <typename T>
void	displayVector(const T &vector, const std::string &name)
{
	for (typename T::const_iterator it = vector.begin(); it != vector.end(); it++)
		std::cout << name + " = " << *it << std::endl;
}

template <typename T>
void	displayMap(const T &map, const std::string &name)
{
	for (typename T::const_iterator it = map.begin(); it != map.end(); it++)
		std::cout << name + " " << it->first << " = " << it->second << std::endl;
}

#endif