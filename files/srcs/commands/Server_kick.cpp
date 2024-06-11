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

/**
	*  @brief	Kick a user from a channel
	*  @param	int fd : file descriptor of the client socket
	*  @param	std::string msg : message received from the client
	*  @return	None
*/
void Server::kick(int fd, std::string const &msg)
{
	std::string channel_name;
	std::string nickname;
	std::string reason;
	std::istringstream iss(msg);

	iss >> channel_name >> nickname >> reason;
	if (channel_name.empty() || nickname.empty())
	{
		servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.find(fd)->second->getNickname(), "KICK"));
		return ;
	}
	else if (reason.empty())
		reason = "No reason given";

	if (_channel.find(channel_name) == _channel.end())
		servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.find(fd)->second->getNickname(), channel_name));
	else if (_channel.find(channel_name)->second.getAdmins().find(nickname) == _channel.find(channel_name)->second.getAdmins().end())
		servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(_client.find(fd)->second->getNickname(), channel_name));
	else if (_channel.find(channel_name)->second.getMembers().find(nickname) == _channel.find(channel_name)->second.getMembers().end())
		servSend(_srv_sock, fd, ERR_NOTONCHANNEL(_client.find(fd)->second->getNickname(), channel_name));
	else
	{
		servSend(_srv_sock, fd, RPL_KICK(channel_name, nickname, reason));
		_channel.find(channel_name)->second.removeMember(nickname);

		if (_channel.find(channel_name)->second.getAdmins().find(nickname) != _channel.find(channel_name)->second.getAdmins().end())
			_channel.find(channel_name)->second.removeAdmin(nickname);

		if (_channel.find(channel_name)->second.getMembers().empty())
			_channel.erase(channel_name);
	}
}
