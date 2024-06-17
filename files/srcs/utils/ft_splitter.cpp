/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitter.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:27:25 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/17 14:27:25 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

t_cmd ft_splitter(std::string const &str)
{
	std::string line;
	std::istringstream iss(str);
	std::vector<std::string> result;

		while (std::getline(iss, line))
		{
			if (!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);
			result.push_back(line);
		}
		return (result);
}
