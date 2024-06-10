/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_mode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 14:36:45 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/10 15:01:43 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::mode(int fd, std::string const &msg)
{
	// std::deque<std::string> args = this->split(msg, " ");
	// if (args.size() < 2)
	// 	return servSend(fd, -1, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "MODE"));
	// if (args[1].at(0) != '#' && !args[1].compare(_client.at(fd)->getNickname()));
	// 	return servSend(fd, -1, RPL_UMODEIS(_client.at(fd)->getNickname(), "+wi"));

	// Channel *channel = 0;

	(void)msg;
	(void)fd;
	
}
