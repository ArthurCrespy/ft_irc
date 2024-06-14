/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_part.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:24:57 by abinet            #+#    #+#             */
/*   Updated: 2024/06/14 19:04:40 by abinet           ###   ########.fr       */
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
				channel.broadcast(client->getNickname(), RPL_PART(client->getNickname(), channel_name));
				channel.removeMember(client);
			}
			else
				servSend(_srv_sock, fd, ERR_NOTONCHANNEL(_client.at(fd)->getNickname(), channel_name));
		}
		else
			servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), channel_name));
	}
}
