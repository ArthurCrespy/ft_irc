/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:49:33 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/04 13:41:26 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"
#include "../../includes/client/Client.hpp"
#include "../../includes/channel/User.hpp"
#include "../../includes/server/Server.hpp"
#include "../../includes/channel/Join.hpp"
#include <deque>
#include <string>

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

std::deque<std::string> Client::split(std::string const &msg, std::string const &delimiters) {
    std::deque<std::string> args;
    std::string token;
    size_t pos = 0;
    std::string message = msg;

    while ((pos = message.find_first_of(delimiters)) != std::string::npos) {
        token = message.substr(0, pos);

        if (!token.empty())
            args.push_back(token);
        message.erase(0, pos + 1);
    }
    if (!message.empty())
        args.push_back(message);
    return args;
}

void Client::ExecJoinTEST(const std::string &msg, int fd, Server & server, User &user)
{
	std::cout << "JE VAIS JOIN ATTENTION" << std::endl;
	// std::deque<std::string>	channels = Client::split(commandArgs[1], ",");
    (void)user;
}

User	*Client::findUserByFd(int fd) {
	std::map<int, User>::iterator	it = this->_users.find(fd);

	if (it == this->_users.end())
		return (NULL);
	return (&it->second);
}

void Client::cliReceive(std::string const &msg, int fd, Server & server)
{
	ft_print("Client received: " + msg, INFO);

	User *user = findUserByFd(fd);
		std::cout << user << std::endl;
	std::string command = msg.substr(0, msg.find(" "));
	std::string remaining = msg.substr(msg.find(" ") + 1);

	
	if ((command == "PART" || command == "MODE" || command == "PRIVMSG" || command == "JOIN" || command == "KICK" || command == "INVITE") &&
		(remaining.empty() || remaining == "\r\n"))
	{
		ft_send(fd, ERR_NEEDMOREPARAMS(getNickname(), command), 0);
		return ;
	}

	if (msg.find("PING") != std::string::npos)
		ft_send(fd, RPL_PONG(getHostname()), 0); // nickname or hostname ?
	else if (msg.find("PRIVMSG") != std::string::npos)
	{
		std::cerr << "cliReceive: PRIVMSG" << std::endl;
		handlePrivMsg(msg, fd);
	}
	else if (msg.find("JOIN") != std::string::npos)
	{
		std::cerr << "cliReceive: JOIN" << std::endl;
		ExecJoinTEST(msg, fd, *this, user);
	}
}

void Client::handlePrivMsg(const std::string &msg, int fd)
{
	size_t start = msg.find("PRIVMSG") + 8;
	size_t end = msg.find(" ", start);

	if (end == std::string::npos)
	{
		ft_send(fd, ERR_NEEDMOREPARAMS(getNickname(), "PRIVMSG"), 0);
		return ;
	}
	std::string name = msg.substr(start, end - start);
	std::string message = msg.substr(end + 1);
	if (message.empty() || message == "\r\n")
	{
		ft_send(fd, ERR_NOTEXTTOSEND(getNickname()), 0);
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
//				 if (/*comparer le nom avec les autres channels existant*/)
//				 {
//				 	if (/*erreur si le message ne s'est pas envoye*/)
//				 		ft_send(fd, ERR_NOTONCHANNEL(getNickname(), channel_name), 0);
//				 }
//				 else
//				 	ft_send(fd, ERR_NOSUCHCHANNEL(getNickname(), channel_name), 0);
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
	std::string str = ":" + getPrefix() + " " + msg + "\r\n";

	if (send(fd, str.c_str(), ft_strlen(str), flags) == -1)
		throw std::runtime_error("Syscall send() Failed in send: " + std::string(std::strerror(errno)));
}
