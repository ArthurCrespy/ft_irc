/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:49:33 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/22 00:52:24 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

Client::Client(void) : _cli_fd(-1), _cli_port(-1) {}

Client::Client(int cli_fd, int cli_port, std::string const &hostname) : _cli_fd(cli_fd), _cli_port(cli_port), _cli_hostname(hostname) {}

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

void Client::cliReceive(std::string const &msg, int fd)
{
	ft_print("Client received: " + msg, INFO);

	std::string command = msg.substr(0, msg.find(" "));
	std::string remaining = msg.substr(msg.find(" ") + 1);

	if ((command == "PART" || command == "MODE" || command == "PRIVMSG" || command == "JOIN" || command == "KICK" || command == "INVITE") &&
		(remaining.empty() || remaining == "\r\n"))
	{
		ft_send(fd, ft_stov(ERR_NEEDMOREPARAMS(getNickname(), command)), 0);
		return ;
	}

	if (msg.find("PING") != std::string::npos)
	{
		std::cout << "dans PING" << std::endl;
		send(fd, RPL_PONG, strlen(RPL_PONG), 0);
	}
	else if (msg.find("PRIVMSG") != std::string::npos)
	{
		std::cout << "dans PRIVMSG" << std::endl;
		handlePrivMsg(msg, fd);
	}
	else if (msg.find("JOIN") != std::string::npos)
	{
		std::cout << "dans JOIN" << std::endl;
		/* besoin d'une classe channel*/
	}
}

void Client::handlePrivMsg(const std::string &msg, int fd)
{
	size_t start = msg.find("PRIVMSG") + 8;
	size_t end = msg.find(" ", start);

	if (end == std::string::npos)
	{
		ft_send(fd, ft_stov(ERR_NEEDMOREPARAMS(getNickname(), "PRIVMSG")), 0);
		return ;
	}
	std::string name = msg.substr(start, end - start);
	std::string message = msg.substr(end + 1);
	if (message.empty() || message == "\r\n")
	{
		ft_send(fd, ERR_NOTEXTTOSEND(getNickname(), name), 0);
		return ;
	}
	if (name[0] != '#' && name[0] != '&')
		msg_prv(fd, name, message);
	else
		msg_channel(fd, name, message);
}

void Client::msg_prv(int fd, const std::string& msg, const std::string& name)
{
	std::string::size_type pos = msg.find(name);
	if (pos != std::string::npos)
	{
		std::string::size_type start = pos + name.length() + 1;
		std::string::size_type end = msg.find("\r\n", start);
		if (end != std::string::npos)
		{
			std::string message = msg.substr(start, end - start);
			ft_send(fd, RPL_PRIVMSG(getNickname(), name, message), 0);
		}
	}
	ft_send(fd, ERR_NOSUCHNICK(getNickname(), name), 0);
}

void Client::msg_channel(int fd, const std::string& msg, const std::string& name)
{
	try
	{
		std::string::size_type start = msg.find(name);
		if (start != std::string::npos)
		{
			start += name.length() + 1;
			std::string::size_type end = msg.find("\r\n", start);
			if (end != std::string::npos)
			{
				std::string message = msg.substr(start, end - start);
				std::string channel_name = name.substr(1);
				// if (/*comparer le nom avec les autres channels existant*/)
				// {
				// 	if (/*erreur si le message ne s'est pas envoye*/)
				// 		send(fd, ERR_NOTONCHANNEL(getNickname(), channel_name).c_str(), ERR_NOTONCHANNEL(getNickname(), channel_name).size(), 0);
				// }
				// else
				// 	send(fd, ERR_NOSUCHCHANNEL(getNickname(), channel_name).c_str(), ERR_NOSUCHCHANNEL(getNickname(), channel_name).size(), 0);
			}
		}
	}
	catch (const std::out_of_range&)
	{
		ft_send(fd, ERR_NOSUCHCHANNEL(getNickname(), name), 0);
	}
}

void Client::ft_send(int fd, std::string const &msg, int flags)
{
//	std::string str = ":" + getPrefix() + " " + msg + "\r\n";
	std::string str = ":localhost " + msg + "\r\n";

	if (send(fd, ft_stov(str), ft_strlen(str), flags) == -1)
		throw std::runtime_error("Syscall send() Failed in send: " + std::string(std::strerror(errno)));

	std::cerr << str << std::endl;
}
