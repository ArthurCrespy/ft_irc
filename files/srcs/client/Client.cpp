/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:49:33 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/03 00:31:57 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

Client::Client(void) : _cli_fd(-1), _cli_port(-1) {}

Client::Client(int cli_fd, int cli_port, std::string const &hostname) : _cli_fd(cli_fd), _cli_port(cli_port), _cli_hostname(hostname)
{
	_cli_nickname = "nickname1";
	_cli_realname = "realname1";
	_cli_username = "username1";
}

Client::Client(Client const &src)
{
	*this = src;
}

Client::~Client(void) {}

Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		this->_cli_fd = rhs._cli_fd;
		this->_cli_port = rhs._cli_port;
		this->_cli_nickname = rhs._cli_nickname;
		this->_cli_username = rhs._cli_username;
		this->_cli_realname = rhs._cli_realname;
		this->_cli_hostname = rhs._cli_hostname;
	}
	return (*this);
}

void Client::cliReceive(std::string const &msg, int fd, Server & server)
{
	ft_print("Client received: " + msg, INFO);

	std::string command = msg.substr(0, msg.find(" "));
	std::string remaining = msg.substr(msg.find(" ") + 1);

	if ((command == "PART" || command == "MODE" || command == "PRIVMSG" || command == "JOIN" || command == "KICK" || command == "INVITE") &&
		(remaining.empty() || remaining == "\r\n"))
	{
		ft_send(fd, ERR_NEEDMOREPARAMS(getNickname(), command), 0);
		return ;
	}

	if (command == "PING")
		ft_send(fd, RPL_PONG(getNickname()), 0); // nickname or hostname ?
	else if (command == "PRIVMSG")
	{
		// std::cerr << "cliReceive: PRIVMSG" << std::endl;
		handlePrivMsg(remaining, fd, server);
	}
	else if (msg.find("JOIN") != std::string::npos)
	{
		handleJoin(remaining, fd, server);
		// std::cerr << "cliReceive: JOIN" << std::endl;
		// Channel
	}
}

void Client::handleJoin(const std::string &msg, int fd, Server & server)
{
	(void)fd;
	(void)server;
	std::string channel = msg;
	if (channel[0] != '#' || channel[0] != '&')
		return ft_send(fd, ERR_NOSUCHCHANNEL(getNickname(), channel), 0);
	channel.erase(0, 1);
	// il faut pouvoir acceder a server pour ensuite acceder aux channels ?
	// c'est pour ca que j'ai mis server en parametre

}

void Client::handlePrivMsg(const std::string &msg, int fd, Server & server)
{
	std::cout << "msg :" << msg << std::endl;
	size_t end = msg.find(" ");
	if (end == std::string::npos)
	{
		ft_send(fd, ERR_NEEDMOREPARAMS(getNickname(), "PRIVMSG"), 0);
		return ;
	}
	std::string name = msg.substr(0, end);
	std::string message = msg.substr(end + 2);
	if (message.empty() || message == "\r\n")
	{
		ft_send(fd, ERR_NOTEXTTOSEND(getNickname()), 0);
		return ;
	}
	if (name[0] != '#' && name[0] != '&')
		msg_prv(fd, message, name);
	else
		msg_channel(fd, message, name, server);
}

void Client::msg_prv(int fd, const std::string& msg, const std::string& name)
{
	ft_send(fd, RPL_PRIVMSG(getNickname(), name, msg), 0);
		return ;
}

void Client::msg_channel(int fd, const std::string& msg, const std::string& name, Server & server)
{
	std::string name_channel = msg;
	if (name_channel[0] != '#' || name_channel[0] != '&')
		return ft_send(fd, ERR_NOSUCHCHANNEL(getNickname(), name_channel), 0);
	name_channel.erase(0, 1);
	try
	{
		Channel channel;
		channel = server.getchannel(name_channel);
		// if (/*envoyer le message, erreur si le message ne s'est pas envoye*/)
			// ft_send(fd, ERR_NOTONCHANNEL(getNickname(), name_channel), 0);
	}
	catch (const std::out_of_range&)
	{
		ft_send(fd, ERR_NOSUCHCHANNEL(getNickname(), name), 0);
	}
}

void Client::ft_send(int fd, std::string const &msg, int flags)
{
	std::string str = ":" + getPrefix() + " " + msg + "\r\n";

	if (send(fd, str.c_str(), ft_strlen(str), flags) == -1)
		throw std::runtime_error("Syscall send() Failed in send: " + std::string(std::strerror(errno)));
}
