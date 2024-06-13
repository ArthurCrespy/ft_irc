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
    if (_user.count(name) == 0)
        return (servSend(_srv_sock, fd, ERR_NOSUCHNICK(_client.at(fd)->getNickname(), msg)));

	servSend(fd, _user.at(name).getFd(), RPL_PRIVMSG(_client.at(_user.at(name).getFd())->getNickname(), msg));
}

void Server::msgChannel(int fd, std::string &channel, std::string const &msg)
{
	if (channel[0] != '#' && channel[0] != '&')
		return (servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), channel)));
	channel.erase(0, 1);

	if (_channel.count(channel) == 0)
		return (servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), channel)));

	if (_channel.at(channel).getMembers().count(_client.at(fd)->getNickname()) == 0)
		return (servSend(_srv_sock, fd, ERR_NOTONCHANNEL(_client.at(fd)->getNickname(), channel)));

	_channel.at(channel).broadcast(_client.at(fd)->getNickname(), RPL_CHANNEL(channel, msg));
}

void Server::msgSend(int fd, const std::string &msg)
{
	std::istringstream iss(msg);
	std::string name;
	std::string message;

	iss >> name;
	std::getline(iss, message);

	if (message.empty() || message == "\r\n")
		return (servSend(_srv_sock, fd, ERR_NOTEXTTOSEND(_client.at(fd)->getNickname())));
	while (message[0] == ' ' || message[0] == ':')
		message.erase(0, 1);
	if (std::find(message.begin(), message.end(), '\r') != message.end())
		message.erase(message.end() - 1);
	if (name[0] != '#' && name[0] != '&')
		msgPrv(fd, name, message);
	else
		msgChannel(fd, name, message);
}
