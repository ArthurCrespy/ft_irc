/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_setup.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:14:01 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/14 16:14:01 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

/**
	* @brief	Set the port and password of the server. Ask if user wants to use default values if not enough arguments are given
	* @param	int argc : number of arguments
	* @param	char **argv : arguments
	* @return	None
*/
void Server::servSetup(int argc, char **argv)
{
	if (argc != 3)
	{
		ft_print("Usage: ./ircserv <port> <password>", WARN);
		ft_print("Would you like to continue with port '6667' and password 'default' ? [Y/N]", INPUT);
		std::string answer;
		std::getline(std::cin, answer);
		if (answer.empty() || (answer != "Y" && answer != "y"))
			throw std::runtime_error("Server not started");
		setPort(6667);
		setPassword("default");
	}
	else
	{
		this->setPort(argv[1]);
		this->setPassword(argv[2]);
	}
}
