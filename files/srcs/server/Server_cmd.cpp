/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:54:01 by abinet            #+#    #+#             */
/*   Updated: 2024/06/06 17:18:43 by abinet           ###   ########.fr       */
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

	// if ((command == "PRIVMSG" || command == "/msg") && remaining.find("LOGBOT") == 1)
	// {
	// 	logBot(fd, remaining);
	// 	return ;
	// }
	// else if (!_client.at(fd)->getRegistration())
	// {
	// 	ft_send(fd, ERR_NOLOGIN(_client.at(fd)->getHostname()), 0);
	// 	return ;
	// }
	if ((command == "PART" || command == "MODE" || command == "PRIVMSG" || command == "JOIN" || command == "KICK" || command == "INVITE" ||
		command == "/part" || command == "/mode" || command == "/msg" || command == "/join" || command == "/kick" || command == "/invite") &&
		(remaining.empty() || remaining == "\r\n"))
		return ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), command), 0);
	if (command == "PING" || command == "/ping")
		ft_send(fd, RPL_PONG(_client.at(fd)->getNickname()), 0); // nickname or hostname ?
	else if (command == "PRIVMSG" || command == "/msg")
		handlePrivMsg(remaining, fd);
	else if (command == "JOIN" || command == "/join")
		handleJoin(remaining, fd);
	else if (command == "INVITE" || command == "/invite")
		handleInvite(remaining, fd);
}

void Server::handleInvite(const std::string &msg, int fd)
{
	std::istringstream iss(msg);
	std::string guest;
	std::string name_channel;
	iss >> guest >> name_channel;

	if(guest.empty() || name_channel.empty())
		return ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "INVITE"), 0);

	if (!_client.at(fd))
		return ft_send(fd, ERR_NOSUCHNICK(_client.at(fd)->getNickname(), guest), 0); // erreur sur guest attention
	try
	{
		_channel.at(name_channel);
	}
	catch (const std::out_of_range&)
	{
		ft_send(fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), name_channel), 0);
	}
	// if (_user.at(guest).getNickname())
	// {
	// }
}

void Server::handleJoin(const std::string &msg, int fd)
{
	std::istringstream iss(msg);

	std::string name_channel;
	std::string message;

	iss >> name_channel;

	if (name_channel.empty())
		return ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN"), 0);
	if (name_channel[0] != '#' && name_channel[0] !='&')
		return ft_send(fd, ERR_BADCHANMASK(_client.at(fd)->getNickname(), name_channel), 0);
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
				return ft_send(fd, ERR_PASSWDMISMATCH(_client.at(fd)->getNickname()), 0);
		}
		_channel.at(name_channel).addMember(_client.at(fd));
		ft_send(fd, "You have joined the channel: " + name_channel, 0);
		_channel.at(name_channel).broadcast(_client.at(fd)->getNickname() + " has joined the channel.");
	}
	else
	{
		Channel newChannel(name_channel, _client.at(fd));
		_channel.insert(std::make_pair(name_channel, newChannel));
		_channel.at(name_channel).addMember(_client.at(fd));
		ft_send(fd, "You have joined the channel: " + name_channel, 0);
	}
}

void Server::handlePrivMsg(const std::string &msg, int fd)
{
	std::istringstream iss(msg);
	std::string name;
	std::string message;

	iss >> name;
	std::getline(iss, message);

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
