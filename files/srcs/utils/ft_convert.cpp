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

#include "./../../includes/ft_irc.h"

int ft_stoi(const std::string &str)
{
	int					number;
	std::istringstream	iss(str);

	if (!(iss >> number))
		throw std::invalid_argument("Invalid input string");

	return (number);
}

std::string ft_nbtos(const int &value)
{
    std::ostringstream oss;

    if (!(oss << value))
		throw std::invalid_argument("Invalid input value");

    return (oss.str());
}
