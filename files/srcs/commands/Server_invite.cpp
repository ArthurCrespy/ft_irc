/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_invite.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:29:47 by abinet            #+#    #+#             */
/*   Updated: 2024/06/13 12:43:13 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::invite(int fd, std::string const &msg)
{
	std::istringstream iss(msg);
	std::string nickname;
	std::string channel_name;

	iss >> nickname >> channel_name;

	if (_user.count(nickname) == 0)
		return (servSend(_srv_sock, fd, ERR_NOSUCHNICK(_client.at(fd)->getNickname(), msg)));
	if (channel_name.empty() || nickname.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.find(fd)->second->getNickname(), "KICK")));

	if (channel_name[0] == '#' || channel_name[0] == '&')
		channel_name.erase(0, 1);

	if(_channel.count(channel_name) == 0)
	{
		Channel newChannel(channel_name, &_user.at(nickname));
		_channel.insert(std::make_pair(channel_name, newChannel));
	}
	else
	{
		if (_channel.at(channel_name).getInviteOnly())
		{
			if (!_channel.at(channel_name).isAdmin(_client.at(fd)))
				return servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(_client.find(fd)->second->getNickname(), channel_name));
		}
		_channel.at(channel_name).addMember(&_user.at(nickname));
		_channel.at(channel_name).broadcast(nickname, nickname + " has joined the channel.");
	}
	servSend(_srv_sock, fd, RPL_INVITING(_client.find(fd)->second->getNickname(), nickname, channel_name));
}
