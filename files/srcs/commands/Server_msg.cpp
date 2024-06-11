/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_msg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:51:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 10:57:03 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::msgPrv(int fd, std::string const &name, std::string const &msg)
{
	try
	{
		servSend(_client.at(fd)->getFd(), _user.at(name).getFd(), RPL_PRIVMSG(_client.at(_user.at(name).getFd())->getNickname(), msg));
	}
	catch (const std::out_of_range &)
	{
		servSend(_srv_sock, fd, ERR_NOSUCHNICK(_client.at(fd)->getNickname(), msg));
	}
}

void Server::msgChannel(int fd, std::string &channel, std::string const &msg)
{
	if (channel[0] != '#' && channel[0] != '&')
		return servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), channel));
	channel.erase(0, 1);
	try
	{
		_channel.at(channel).getMembers().find(_client.at(fd)->getNickname());
		try
		{
			_channel.at(channel).broadcast(_client.at(fd)->getNickname(), msg);
		}
		catch (const std::out_of_range &)
		{
			servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), channel));
		}
	}
	catch (const std::out_of_range &)
	{
		servSend(_srv_sock, fd, ERR_NOTONCHANNEL(_client.at(fd)->getNickname(), channel));
	}
}

void Server::msgSend(int fd, const std::string &msg)
{
	std::istringstream iss(msg);
	std::string name;
	std::string message;

	iss >> name;
	std::getline(iss, message);
	std::cout << "message :" << message << std::endl;

	if (message.empty() || message == "\r\n")
	{
		servSend(_srv_sock, fd, ERR_NOTEXTTOSEND(_client.at(fd)->getNickname()));
		return ;
	}
	while (message[0] == ' ' || message[0] == ':')
		message.erase(0, 1);
	if (name[0] != '#' && name[0] != '&')
		msgPrv(fd, name, message);
	else
		msgChannel(fd, name, message);
}
