/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:08:39 by auzun             #+#    #+#             */
/*   Updated: 2023/05/09 15:29:53 by ilandols         ###   ########.fr       */
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

size_t									rfind(const std::string& str, const std::string& substr);
std::string								to_string( size_t );
std::vector<std::string>				fileToVector( std::string );

bool	closeBrace(std::string &src, const int index);
bool	openBrace(std::string &src, const int index);

std::string					findInFileContent(const std::vector<std::string> &, const std::string &);
std::vector<std::string>	multipleFindInFileContent(const std::vector<std::string> &, const std::string &);

bool	isValidIP( const std::string & );

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
