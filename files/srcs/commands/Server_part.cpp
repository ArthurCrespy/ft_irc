/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_part.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:24:57 by abinet            #+#    #+#             */
/*   Updated: 2024/06/17 14:14:08 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::part(int fd, std::string const &msg)
{
	std::istringstream iss(msg);
	std::string channel_name;
	Client *client = _client.at(fd);

	while (iss >> channel_name)
	{
		if (channel_name.empty())
				return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(client->getNickname(), "KICK")));
		channel_name.erase(0, 1);
		if (_channel.count(channel_name) != 0)
		{
			Channel &channel = _channel.at(channel_name);
			if (channel.isMember(client))
			{
				channel.broadcast(client->getNickname(), RPL_PART(channel_name));
				servSend(fd, fd, RPL_PART(channel_name));
				channel.removeMember(client);
				channel.removeAdmin(client);
				if (channel.getMembers().empty())
				{
					_channel.erase(channel_name);
					ft_print("Channel deleted: " + channel_name, LOG);
				}
			}
			else
				servSend(_srv_sock, fd, ERR_NOTONCHANNEL(client->getNickname(), channel_name));
		}
		else
			servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(client->getNickname(), channel_name));
	}
}
