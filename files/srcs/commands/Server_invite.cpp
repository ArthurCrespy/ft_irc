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
	std::string nickname;
	std::string channel_name;
	std::istringstream iss(msg);

	iss >> nickname >> channel_name;
	if (channel_name[0] == '#' || channel_name[0] == '&')
		channel_name.erase(0, 1);
	if (channel_name.empty() || nickname.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.find(fd)->second->getNickname(), "KICK")));

	if (!isClient(nickname))
		servSend(_srv_sock, fd, ERR_NOSUCHNICK(_client.at(fd)->getNickname(), msg));
	else if (_channel.count(channel_name) != 0)
	{
		Channel &channel = _channel.at(channel_name);
		if (channel.getInviteOnly())
		{
			if (!channel.isAdmin(_client.at(fd)))
				return (servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(_client.find(fd)->second->getNickname(), channel_name)));
		}
		channel.addMember(&getClient(nickname));
		channel.broadcast(nickname, nickname + " has joined the channel.");
	}
	else
	{
        Channel channel_new(channel_name, &getClient(nickname));
		_channel.insert(std::make_pair(channel_name, channel_new));
	}
	servSend(_srv_sock, fd, RPL_INVITING(_client.find(fd)->second->getNickname(), nickname, channel_name));
}
