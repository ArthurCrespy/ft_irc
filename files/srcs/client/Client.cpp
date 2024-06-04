/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:49:33 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/03 19:53:12 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

Client::Client(void) : _cli_fd(-1), _cli_port(-1), _cli_registered(false) {}

Client::Client(int cli_fd, int cli_port, std::string const &hostname) : _cli_fd(cli_fd), _cli_port(cli_port), _cli_registered(false), _cli_hostname(hostname) {}

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
		this->_cli_registered = rhs._cli_registered;
		this->_cli_nickname = rhs._cli_nickname;
		this->_cli_username = rhs._cli_username;
		this->_cli_realname = rhs._cli_realname;
		this->_cli_hostname = rhs._cli_hostname;
	}
	return (*this);
}

void Client::ft_send(int fd, std::string const &msg, int flags)
{
	std::string str = ":" + getPrefix() + " " + msg + "\r\n";

	if (send(fd, str.c_str(), ft_strlen(str), flags) == -1)
		throw std::runtime_error("Syscall send() Failed in send: " + std::string(std::strerror(errno)));
}
