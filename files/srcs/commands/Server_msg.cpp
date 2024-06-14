/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_msg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:51:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/13 17:49:18 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::msgPrv(int fd, std::string const &name, std::string const &msg)
{
	if (!isClient(name))
		servSend(_srv_sock, fd, ERR_NOSUCHNICK(_client.at(fd)->getNickname(), msg));
	else
		servSend(fd, getClient(name).getFd(), RPL_PRIVMSG(_client.at(getClient(name).getFd())->getNickname(), msg));
}

void Server::msgChannel(int fd, std::string &channel_name, std::string const &msg)
{
	if (channel_name[0] != '#' && channel_name[0] != '&')
		return (servSend(_srv_sock, fd, ERR_BADCHANMASK(channel_name)));
	else 
		channel_name.erase(0, 1);

	std::string nickname = _client.at(fd)->getNickname();
	if (_channel.count(channel_name) != 0)
	{
		Channel &channel = _channel.at(channel_name);
		if (channel.getMembers().count(nickname) == 0)
			servSend(_srv_sock, fd, ERR_NOTONCHANNEL(nickname, channel_name));
		else
			channel.broadcast(nickname, RPL_CHANNEL(channel_name, msg));
	}
	else
		servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(nickname, channel_name));
}

void Server::msgSend(int fd, std::string const &msg)
{
	std::string name;
	std::string message;
	std::istringstream iss(msg);

	iss >> name;
	std::getline(iss, message);

	while (message[0] == ' ' || message[0] == ':')
		message.erase(0, 1);
	if (message.empty() || message == "\r\n")
		return (servSend(_srv_sock, fd, ERR_NOTEXTTOSEND(_client.at(fd)->getNickname())));

	if (name[0] != '#' && name[0] != '&')
		msgPrv(fd, name, message);
	else
		msgChannel(fd, name, message);
}
