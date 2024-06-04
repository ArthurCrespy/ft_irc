/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:54:01 by abinet            #+#    #+#             */
/*   Updated: 2024/06/04 00:26:40 by abinet           ###   ########.fr       */
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

//	 std::string command = msg.substr(0, msg.find(" "));
//	 std::string remaining = msg.substr(msg.find(" ") + 1);

//	if (!_client.at(fd)->getRegistration())
//	{
//		if ((command.find("PRIVMSG") != std::string::npos || command.find("/msg") != std::string::npos) && remaining.find("LOG") != std::string::npos)
//			logBot(fd, remaining);
//		else
//			ft_send(_client.at(fd)->getFd(), ERR_NOTREGISTERED(_client.at(fd)->getHostname()), 0);
//		return ;
//	}

	if ((command == "PART" || command == "MODE" || command == "PRIVMSG" || command == "JOIN" || command == "KICK" || command == "INVITE") &&
		(remaining.empty() || remaining == "\r\n"))
	{
		ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), command), 0);
		return ;
	}

	if (command == "PING")
		ft_send(fd, RPL_PONG(_client.at(fd)->getNickname()), 0); // nickname or hostname ?
	else if (command == "PRIVMSG")
		handlePrivMsg(remaining, fd);
	else if (msg.find("JOIN") != std::string::npos) {}
		// handleJoin(remaining, fd, server);
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
	std::string target;
	std::string message;

	//to fix : name_channel, target, msg

	iss >> target;
	std::getline(iss, message);

	if (message.empty() || message == "\r\n")
	{
		ft_send(fd, ERR_NOTEXTTOSEND(_client.at(fd)->getNickname()), 0);
		return ;
	}
	while (message[0] == ' ' || message[0] == ':')
		message.erase(0, 1);
	// std::cout << "target :" << target << std::endl;
	// std::cout << "message :" << message << std::endl;
	if (target[0] != '#' && target[0] != '&')
		msg_prv(fd, message, target);
	else
		msg_channel(fd, message, target);
}

void Server::msg_prv(int fd, const std::string& msg, const std::string& name)
{
	ft_send(fd, RPL_PRIVMSG(_client.at(fd)->getNickname(), name, msg), 0);
}

void Server::msg_channel(int fd, const std::string& msg, const std::string& target)
{
	//erreur target est en fait le name_channel et msg est juste le msg
	// il faut recup la target et le name_channel avant
	std::cout << "target :" << target << std::endl;
	std::string name_channel = msg;
	if (name_channel[0] != '#' || name_channel[0] != '&')
		return ft_send(fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), name_channel), 0);
	name_channel.erase(0, 1);
	try
	{
		ft_send(fd, msg, 0);
		// Channel channel;
		// channel = getchannel(name_channel);
		// if (/*envoyer le message, erreur si le message ne s'est pas envoye*/)
			// ft_send(fd, ERR_NOTONCHANNEL(getNickname(), name_channel), 0);
	}
	catch (const std::out_of_range&)
	{
		ft_send(fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), target), 0);
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
