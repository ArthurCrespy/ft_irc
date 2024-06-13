/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:52:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/13 16:18:42 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::join(int fd, std::string const &msg)
{
	std::istringstream iss(msg);

	std::string channel_name;
	std::string message;

	iss >> channel_name;

	if (channel_name.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN")));
	if (channel_name[0] != '#' && channel_name[0] != '&')
		return (servSend(_srv_sock, fd, ERR_BADCHANMASK(channel_name)));
	channel_name.erase(0, 1);

	it_channel it;
	it = _channel.find(channel_name);
	if (it != _channel.end())
	{
		if (_channel.at(channel_name).getPasswordRestriction())
		{
			std::string mdp;
			iss >> mdp;
			if (mdp.empty() || _channel.at(channel_name).getPassword() != mdp)
				return (servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(_client.at(fd)->getNickname())));
		}

		if (_channel.at(channel_name).getMembers().count(_client.at(fd)->getNickname()) != 0)
			return (servSend(_srv_sock, fd, _client.at(fd)->getNickname() + ": already in the channel")); // pas trouve le msg a renvoye dans ce genre de cas

		if (_channel.at(channel_name).hasMode('l') && (int)_channel.at(channel_name).getMembers().size() >= _channel.at(channel_name).getLimit())
			return (servSend(_srv_sock, fd, ERR_CHANNELISFULL(_client.at(fd)->getNickname(), channel_name)));
		if (_channel.at(channel_name).getInviteOnly())
				return (servSend(_srv_sock, fd, ERR_INVITEONLYCHAN(_client.at(fd)->getNickname(), channel_name)));

		_channel.at(channel_name).addMember(_client.at(fd));
		servSend(_srv_sock, fd, "You have joined the channel: " + channel_name);
		_channel.at(channel_name).broadcast(_client.at(fd)->getNickname(), _client.at(fd)->getNickname() + " has joined the channel.");
	}
	else
	{
		Channel newChannel(channel_name, _client.at(fd));
		_channel.insert(std::make_pair(channel_name, newChannel));
		servSend(_srv_sock, fd, "You have joined the channel: " + channel_name);
	}
}
