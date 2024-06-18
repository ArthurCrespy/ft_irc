/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_invite.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:29:47 by abinet            #+#    #+#             */
/*   Updated: 2024/06/14 16:12:27 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::invite(int fd, std::string const &msg)
{
	std::string guest_name;
	std::string channel_name;
	std::istringstream iss(msg);
	Client *client = _client.at(fd);

	iss >> guest_name >> channel_name;
	if (channel_name[0] == '#' || channel_name[0] == '&')
		channel_name.erase(0, 1);
	if (channel_name.empty() || guest_name.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(client->getNickname(), "KICK")));

	if (!isClient(guest_name))
		servSend(_srv_sock, fd, ERR_NOSUCHNICK(client->getNickname(), msg));
	else if (_channel.count(channel_name) != 0)
	{
		Channel &channel = _channel.at(channel_name);
		if (channel.getInviteOnly())
		{
			if (!channel.isAdmin(client))
				return (servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(client->getNickname(), channel_name)));
		}
		channel.addInvite(&getClient(guest_name));
		servSend(fd, fd, RPL_INVITING(client->getNickname(), channel_name, guest_name));
		servSend(fd, getClient(guest_name).getFd(), RPL_INVITING(client->getNickname(), channel_name, guest_name));
	}
	else
    	servSend(_srv_sock, fd, ERR_NOTONCHANNEL(client->getNickname(), channel_name));
}
