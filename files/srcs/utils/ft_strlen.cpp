/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:26:49 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/29 10:26:49 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

/**
	* @brief	Compute the length of a string
	* @param	std::string str : The string to compute the length
	* @return	The length of the string
*/

size_t ft_strlen(std::string const &str)
{
	return (str.size());
}