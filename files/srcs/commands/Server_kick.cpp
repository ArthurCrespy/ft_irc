/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_kick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:02:42 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/06 11:02:44 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::kick(int fd, std::string const &msg)
{
	std::string channel_name;
	std::string nickname;
	std::string reason;
	std::istringstream iss(msg);

	iss >> channel_name >> nickname >> reason;
	if (channel_name.empty() || nickname.empty())
	{
		ft_send(fd, ERR_NEEDMOREPARAMS(_client.find(fd)->second->getNickname(), "KICK"), 0);
		return ;
	}
	else if (reason.empty())
		reason = "No reason given";

	if (_channel.find(channel_name) == _channel.end())
		ft_send(fd, ERR_NOSUCHCHANNEL(_client.find(fd)->second->getNickname(), channel_name), 0);
	else if (_channel.find(channel_name)->second.getAdmins().find(nickname) == _channel.find(channel_name)->second.getAdmins().end())
		ft_send(fd, ERR_CHANOPRIVSNEEDED(_client.find(fd)->second->getNickname(), channel_name), 0);
	else if (_channel.find(channel_name)->second.getMembers().find(nickname) == _channel.find(channel_name)->second.getMembers().end())
		ft_send(fd, ERR_NOTONCHANNEL(_client.find(fd)->second->getNickname(), channel_name), 0);
	else
	{
		ft_send(fd, RPL_KICK(_client.find(fd)->second->getNickname(), channel_name, nickname, reason), 0);
		_channel.find(channel_name)->second.removeMember(nickname);
		if (_channel.find(channel_name)->second.getMembers().empty())
			_channel.erase(channel_name);
	}
}
