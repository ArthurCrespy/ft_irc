/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stoi.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:27:18 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/14 15:27:18 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

/**
	* @brief	Convert a number to a string
	* @param	int value : The number to convert
	* @return	std::string : The string representation of the number
	* @throw	std::invalid_argument If the input value is invalid
 */
std::string ft_nbtos(int const &value)
{
	std::ostringstream oss;

	if (!(oss << value))
		throw std::invalid_argument("Invalid input value");

	return (oss.str());
}

/**
	* @brief	Convert a string to a number
	* @param	std::string str : The string to convert
	* @return	int : The number representation of the string
	* @throw	std::invalid_argument If the input string is invalid
 */
int ft_stoi(std::string const &str)
{
	int					number;
	std::istringstream	iss(str);

	if (!(iss >> number))
		throw std::invalid_argument("Invalid input string");

	return (number);
}
