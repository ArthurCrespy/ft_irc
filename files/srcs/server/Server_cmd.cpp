/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:54:01 by abinet            #+#    #+#             */
/*   Updated: 2024/06/04 17:38:54 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::handleCommand(std::string const &msg, int fd)
{
	ft_print("Client received: " + msg, INFO);

	std::istringstream iss(msg);
	std::string command;
	iss >> command;

	std::string remaining;
	std::getline(iss, remaining);

	if (command == "PING" || command == "/ping")
		return (ft_send(fd, RPL_PONG(_client.at(fd)->getNickname()), 0));
	if ((command == "PRIVMSG" || command == "/msg") && remaining.find("LOGBOT") == 1)
		return (logBot(fd, remaining));

	if (!_client.at(fd)->getRegistration())
		return (ft_send(fd, ERR_NOLOGIN(_client.at(fd)->getHostname()), 0));

	if ((remaining.empty() || remaining == "\r\n"))
		return (ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), command), 0));


	if (command == "PRIVMSG" || command == "/msg")
		handlePrivMsg(remaining, fd);
	else if (command == "JOIN" || command == "/join")
		{} // handleJoin(remaining, fd, server);
}

void Server::handleJoin(const std::string &msg, int fd)
{
	std::string channel = msg;
	if (channel[0] != '#' || channel[0] != '&')
		return (ft_send(fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), channel), 0));
	channel.erase(0, 1);
}

void Server::handlePrivMsg(const std::string &msg, int fd)
{
	std::istringstream iss(msg);
	std::string name;
	std::string message;

	iss >> name;
	std::getline(iss, message);
	std::cout << "message :" << message << std::endl;

	if (message.empty() || message == "\r\n")
	{
		ft_send(fd, ERR_NOTEXTTOSEND(_client.at(fd)->getNickname()), 0);
		return ;
	}
	while (message[0] == ' ' || message[0] == ':')
		message.erase(0, 1);
	if (name[0] != '#' && name[0] != '&')
		msg_prv(fd, message, name);
	else
		msg_channel(fd, message, name);
}

void Server::msg_prv(int fd, const std::string& msg, const std::string& name)
{
	try
	{
		int dest_fd = _user.at(name).getFd();
		ft_send(dest_fd, RPL_PRIVMSG(_client.at(dest_fd)->getNickname(), name, msg), 0);
	}
	catch(const std::out_of_range&)
	{
		ft_send(fd, ERR_NOSUCHNICK(_client.at(fd)->getNickname(), name), 0);
	}
}

void Server::msg_channel(int fd, const std::string& msg, std::string& name_channel)
{
	if (name_channel[0] != '#' && name_channel[0] != '&')
		return ft_send(fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), name_channel), 0);
	name_channel.erase(0, 1);
	try
	{
		_channel.at(name_channel).getMembers().find(_client.at(fd)->getNickname());
		try
		{
			_channel.at(name_channel).broadcast(msg);
		}
		catch (const std::out_of_range&)
		{
			ft_send(fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), name_channel), 0);
		}
	}
	catch (const std::out_of_range&)
	{
		ft_send(fd, ERR_NOTONCHANNEL(_client.at(fd)->getNickname(), name_channel), 0);
	}
}
