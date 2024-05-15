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

std::string ft_nbtos(int const &value)
{
	std::ostringstream oss;

	if (!(oss << value))
		throw std::invalid_argument("Invalid input value");

	return (oss.str());
}

int ft_stoi(std::string const &str)
{
	int					number;
	std::istringstream	iss(str);

	if (!(iss >> number))
		throw std::invalid_argument("Invalid input string");

	return (number);
}
