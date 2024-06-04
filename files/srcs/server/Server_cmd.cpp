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

	if (!_client.at(fd)->getRegistration() && false)
	{
		if ((command.find("PRIVMSG") != std::string::npos || command.find("/msg") != std::string::npos) && remaining.find("LOG") != std::string::npos)
			logBot(fd, remaining);
		else
			ft_send(_client.at(fd)->getFd(), ERR_NOLOGIN(_client.at(fd)->getHostname()), 0);
		return ;
	}

	if ((command == "PART" || command == "MODE" || command == "PRIVMSG" || command == "JOIN" || command == "KICK" || command == "INVITE" ||
		command == "/part" || command == "/mode" || command == "/msg" || command == "/join" || command == "/kick" || command == "/invite") &&
		(remaining.empty() || remaining == "\r\n"))
	{
		ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), command), 0);
		return ;
	}

	if (command == "PING" || command == "/ping")
		ft_send(fd, RPL_PONG(_client.at(fd)->getNickname()), 0); // nickname or hostname ?
	else if (command == "PRIVMSG" || command == "/msg")
		handlePrivMsg(remaining, fd);
	else if (command == "JOIN" || command == "/join")
	{
		// handleJoin(remaining, fd, server);
	}
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
	// while (iss)
	// {
	// 	std::string temp;
	// 	iss >> temp;
	// 	message = message + " " + temp;
	// }
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

void Server::ft_send(int fd, std::string const &msg, int flags)
{
	std::string str = ":" + _client.at(fd)->getPrefix() + " " + msg + "\r\n";

	if (send(fd, str.c_str(), ft_strlen(str), flags) == -1)
		throw std::runtime_error("Syscall send() Failed in send: " + std::string(std::strerror(errno)));
}

void Server::logBot(int fd, std::string const &msg)
{
	std::istringstream iss(msg);
	std::string command, param, password, nickname, username, realname;

	iss >> command >> param >> password >> nickname;

	if (command != "LOG")
	{
		ft_send(fd, ERR_UNKNOWNCOMMAND(_client.at(fd)->getNickname(), command), 0);
		return ;
	}

	if (param.empty())
		ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "LOG"), 0);
	else if (param == "HELP" || param == ":HELP")
		ft_send(fd, RPL_LOGHELP, 0);
	else if (param == "REGISTER" || param == ":REGISTER")
	{
		iss >> username >> realname;
		if (username.empty() || realname.empty())
			ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "REGISTER"), 0);
		else
		{
			_client.at(fd)->setNickname(nickname);
			_client.at(fd)->setUsername(username);
			_client.at(fd)->setRealname(realname);
			_client.at(fd)->setRegistration(true);
			ft_send(fd, RPL_LOGREGISTER(_client.at(fd)->getNickname()), 0);
		}
	}
	else if (param == "LOGIN" || param == ":LOGIN")
	{
		if (password.empty() || nickname.empty())
			ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "LOGIN"), 0);
		else if ( nickname == _client.at(fd)->getNickname() && password == _client.at(fd)->getPassword())
			ft_send(fd, RPL_LOGLOGIN(_client.at(fd)->getNickname()), 0);
		else
			ft_send(fd, ERR_PASSWDMISMATCH(_client.at(fd)->getNickname()), 0);
	}
	else
		ft_send(fd, ERR_UNKNOWNCOMMAND(_client.at(fd)->getNickname(), command), 0);
}
