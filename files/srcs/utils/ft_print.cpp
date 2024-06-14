/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:48:22 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/14 13:48:22 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

std::string ft_time(void)
{
	char		timeString[100];
	std::tm		*localTime;
	std::time_t	currentTime;

	std::time(&currentTime);
	localTime = std::localtime(&currentTime);
	std::strftime(timeString, sizeof(timeString), "%H:%M:%S", localTime);

	return ((std::string)timeString);
}

/**
	* @brief	Print a message with a timestamp, the server name and the level
	* @param	std::string str : The message to print
	* @param	std::string level : The level of the message
	* @return	None
 */
void	ft_print(std::string const &str, std::string const &level)
{
	if (level == INPUT)
		std::cout << "[" << ft_time() << "] (ircserv) "<< level << " > " << str << " : ";
	else
		std::cout << "[" << ft_time() << "] (ircserv) "<< level << " > " << str << std::endl;
}
