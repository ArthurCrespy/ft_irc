/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_mode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 14:36:45 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 15:19:42 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

Channel	*Server::findchannelname(std::string name) {
	t_channel::iterator	it = this->_channel.find(name);

	if (it == this->_channel.end())
		return (NULL);
	return &(it->second);
}

void Server::mode(int fd, std::string const &msg)
{
	std::deque<std::string> args = this->split(msg, " ");
	if (args.size() < 2)
		return servSend(fd, -1, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "MODE"));
	if (args[1].at(0) != '#' && !args[1].compare(_client.at(fd)->getNickname()))
		return servSend(fd, -1, RPL_UMODEIS(_client.at(fd)->getNickname(), "+wi"));

	Channel *channel = this->findchannelname(args[1]);

	if (!channel)
		return servSend(fd, -1, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), args[1]));
	// if (!channel->isInChannel(_client.at(fd), fd))
	// 	return servSend(fd, -1, ERR_NOTONCHANNEL(_client.at(fd)->getNickname(), args[1]));
	if (args.size() == 2) {
		return servSend(fd, -1, RPL_CHANNELMODEIS(_client.at(fd)->getNickname(), args[1], channel->getMode()));
	}
	if (!channel->isAdmins(_client.at(fd)))
		return servSend(fd, -1, ERR_CHANOPRIVSNEEDED(_client.at(fd)->getNickname(), channel->getName()));
	
	std::string	modes = args[2];
	std::string	availableModes = "itkol+-";
	bool		updatedModes = false;

	if (availableModes.find(modes[0]) == std::string::npos)
		return servSend(fd, -1, ERR_UNKNOWNMODE(_client.at(fd)->getNickname(), toString(modes[0])));
	
	(void)updatedModes;
	(void)msg;
	(void)fd;
	
}
