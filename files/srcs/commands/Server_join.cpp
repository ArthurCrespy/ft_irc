/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:52:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/17 14:10:12 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::join(int fd, std::string const &msg)
{
	std::string channel_name;
	std::istringstream iss(msg);
	Client *client = _client.at(fd);

	iss >> channel_name;
	if (channel_name[0] == ':')
		channel_name.erase(0, 1);
	if (channel_name.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN")));
	if (channel_name[0] != '#' && channel_name[0] != '&')
		return (servSend(_srv_sock, fd, ERR_BADCHANMASK(channel_name)));
	else
		channel_name.erase(0, 1);
	if (channel_name.size() == 0 || channel_name.size() > 200 || channel_name.find(',') != std::string::npos || channel_name.find("^G") != std::string::npos)
		return (servSend(_srv_sock, fd, ERR_BADCHANMASK(channel_name)));

	if (_channel.count(channel_name) != 0)
	{
		Channel &channel = _channel.at(channel_name);
		if (channel.getPasswordRestriction())
		{
			std::string mdp;
			iss >> mdp;
			if (mdp.empty() || channel.getPassword() != mdp)
				return (servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(client->getNickname())));
		}

		if (channel.getMembers().count(client->getNickname()) != 0)
			return (servSend(_srv_sock, fd, ERR_USERONCHANNEL(client->getNickname(), channel_name)));
		if (channel.getLimit() > 0 && (int)channel.getMembers().size() >= channel.getLimit())
			return (servSend(_srv_sock, fd, ERR_CHANNELISFULL(client->getNickname(), channel_name)));
		if (channel.getInviteOnly() && !channel.isInvited(client))
				return (servSend(_srv_sock, fd, ERR_INVITEONLYCHAN(client->getNickname(), channel_name)));
		if (channel.isInvited(client))
			channel.removeInvite(client);

		channel.addMember(client);
		servSend(fd, fd, RPL_JOIN(client->getNickname(), channel_name));
		channel.broadcast(client->getNickname(), RPL_JOIN(client->getNickname(), channel_name));
	}
	else
	{
		Channel channel_new(channel_name, client);
		_channel.insert(std::make_pair(channel_name, channel_new));
		servSend(fd, fd, RPL_JOIN(client->getNickname(), channel_name));
	}
}
