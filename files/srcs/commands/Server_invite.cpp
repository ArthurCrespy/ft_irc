/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_invite.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:29:47 by abinet            #+#    #+#             */
/*   Updated: 2024/06/13 17:46:29 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::invite(int fd, std::string const &msg)
{
	std::string guestname;
	std::string channel_name;
	std::istringstream iss(msg);

	iss >> guestname >> channel_name;
	if (channel_name[0] == '#' || channel_name[0] == '&')
		channel_name.erase(0, 1);
	if (channel_name.empty() || guestname.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.find(fd)->second->getNickname(), "KICK")));

	if (!isClient(guestname))
		servSend(_srv_sock, fd, ERR_NOSUCHNICK(_client.at(fd)->getNickname(), msg));
	else if (_channel.count(channel_name) != 0)
	{ 
		Channel &channel = _channel.at(channel_name);
		if (channel.getInviteOnly())
		{
			if (!channel.isAdmin(_client.at(fd)))
				return (servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(_client.find(fd)->second->getNickname(), channel_name)));
		}
		channel.addInvite(&getClient(guestname));
		servSend(fd, fd, RPL_INVITING(_client.find(fd)->second->getNickname(), channel_name, guestname));
		servSend(fd, getClient(guestname).getFd(), RPL_INVITING(_client.find(fd)->second->getNickname(), channel_name, guestname));
	}
	else
    	servSend(_srv_sock, fd, ERR_NOTONCHANNEL(_client.find(fd)->second->getNickname(), channel_name));
}
