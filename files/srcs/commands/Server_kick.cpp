/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_kick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:02:42 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/14 16:11:17 by abinet           ###   ########.fr       */
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
	std::string reason;
	std::string nickname;
	std::string channel_name;
	std::istringstream iss(msg);

	iss >> channel_name >> nickname;
	std::getline(iss, reason);

	if (channel_name[0] == '#' || channel_name[0] == '&')
		channel_name.erase(0, 1);
	if (channel_name.empty() || nickname.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.find(fd)->second->getNickname(), "KICK")));
	while (reason[0] == ' ' || reason[0] == ':')
		reason.erase(0, 1);
	if (reason.empty())
		reason = "No reason given";

	if (_channel.count(channel_name) != 0)
	{
		Channel &channel = _channel.at(channel_name);
		Client *client = _client.at(fd);
		if (channel.isAdmin(_client.at(fd)) == 0)
			servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(client->getNickname(), channel_name));
		else if (channel.getMembers().find(nickname) == channel.getMembers().end())
			servSend(_srv_sock, fd, ERR_NOTONCHANNEL(client->getNickname(), channel_name));
		else
		{
			servSend(fd, fd, RPL_KICK(channel_name, nickname, reason));
			channel.broadcast(client->getNickname(), RPL_KICK(channel_name, nickname, reason));
			channel.removeMember(nickname);
			channel.removeAdmin(nickname);

			if (channel.getMembers().empty() && channel.getAdmins().empty())
				_channel.erase(channel_name);
		}
	}
	else
		servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.find(fd)->second->getNickname(), channel_name));
}
