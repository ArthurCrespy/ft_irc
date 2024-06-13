/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:52:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 11:58:06 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::join(int fd, std::string const &msg)
{
	std::istringstream iss(msg);

	std::string channel_name;
	std::string message;

	iss >> channel_name;

	if (channel_name[0] == ':')
		channel_name.erase(0, 1);
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
		if (_channel.at(channel_name).hasMode('l') && (int)_channel.at(channel_name).getMembers().size() >= _channel.at(channel_name).getLimit())
			return (servSend(_srv_sock, fd, ERR_CHANNELISFULL(_client.at(fd)->getNickname(), channel_name)));
		if (_channel.at(channel_name).hasMode('i') && _channel.at(channel_name).getMembers().find(_client.at(fd)->getNickname()) == _channel.at(channel_name).getMembers().end())
			return (servSend(_srv_sock, fd, ERR_INVITEONLYCHAN(_client.at(fd)->getNickname(), channel_name)));
		_channel.at(channel_name).addMember(_client.at(fd));
		servSend(fd, fd, RPL_JOIN(_client.at(fd)->getNickname(), channel_name));
		_channel.at(channel_name).broadcast(_client.at(fd)->getNickname(), RPL_JOIN(_client.at(fd)->getNickname(), channel_name));
	}
	else
	{
		Channel newChannel(channel_name, _client.at(fd));
		_channel.insert(std::make_pair(channel_name, newChannel));
		servSend(fd, fd, RPL_JOIN(_client.at(fd)->getNickname(), channel_name));
	}
}
