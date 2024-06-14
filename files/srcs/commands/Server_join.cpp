/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:52:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/13 16:54:12 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::join(int fd, std::string const &msg)
{
	std::string channel_name;
	std::istringstream iss(msg);

	iss >> channel_name;
	if (channel_name[0] == ':')
		channel_name.erase(0, 1);
	if (channel_name.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN")));
	if (channel_name[0] != '#' && channel_name[0] != '&')
		return (servSend(_srv_sock, fd, ERR_BADCHANMASK(channel_name)));
	else
		channel_name.erase(0, 1);

	if (_channel.count(channel_name) != 0)
	{
		Channel &channel = _channel.at(channel_name);
		if (channel.getPasswordRestriction())
		{
			std::string mdp;
			iss >> mdp;
			if (mdp.empty() || channel.getPassword() != mdp)
				return (servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(_client.at(fd)->getNickname())));
		}

		if (channel.getMembers().count(_client.at(fd)->getNickname()) != 0)
			return (servSend(_srv_sock, fd, ERR_USERONCHANNEL(_client.at(fd)->getNickname(), channel_name)));

		if (channel.hasMode('l') && (int)channel.getMembers().size() >= channel.getLimit())
			return (servSend(_srv_sock, fd, ERR_CHANNELISFULL(_client.at(fd)->getNickname(), channel_name)));
		if (channel.getInviteOnly())
				return (servSend(_srv_sock, fd, ERR_INVITEONLYCHAN(_client.at(fd)->getNickname(), channel_name)));

		channel.addMember(_client.at(fd));
		servSend(fd, fd, RPL_JOIN(_client.at(fd)->getNickname(), channel_name));
		channel.broadcast(_client.at(fd)->getNickname(), RPL_JOIN(_client.at(fd)->getNickname(), channel_name));
	}
	else
	{
		Channel newChannel(channel_name, _client.at(fd));
		_channel.insert(std::make_pair(channel_name, newChannel));
		servSend(fd, fd, RPL_JOIN(_client.at(fd)->getNickname(), channel_name));
	}
}
