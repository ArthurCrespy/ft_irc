/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:52:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/09 12:52:43 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::join(int fd, std::string const &msg)
{
	std::istringstream iss(msg);

	std::string name_channel;
	std::string message;

	iss >> name_channel;

	if (name_channel.empty())
		return servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN"));
	if (name_channel[0] != '#' && name_channel[0] !='&')
		return servSend(_srv_sock, fd, ERR_BADCHANMASK(name_channel));
	name_channel.erase(0, 1);

	it_channel it;
	it = _channel.find(name_channel);
	if (it != _channel.end())
	{
		if (_channel.at(name_channel).getPasswordRestriction())
		{
			std::string mdp;
			iss >> mdp;
			if (mdp.empty() || _channel.at(name_channel).getPassword() != mdp)
				return servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(_client.at(fd)->getNickname()));
		}
		_channel.at(name_channel).addMember(_client.at(fd));
		servSend(_srv_sock, fd, "You have joined the channel: " + name_channel);
		_channel.at(name_channel).broadcast(_client.at(fd)->getNickname(), _client.at(fd)->getNickname() + " has joined the channel.");
	}
	else
	{
		Channel newChannel(name_channel, _client.at(fd));
		_channel.insert(std::make_pair(name_channel, newChannel));
		servSend(_srv_sock, fd, "You have joined the channel: " + name_channel);
	}
	// std::getline(iss, message); // normalement ca ne sert a rien si il y a d'autres messages apres
}
