/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:53:14 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/16 16:53:14 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void	Client::setFd(int fd)
{
	this->_cli_fd = fd;
}

void	Client::setPort(int port)
{
	this->_cli_port = port;
}

void	Client::setRegistration(bool reg)
{
	this ->_cli_registered = reg;
}

void	Client::setNickname(std::string const &nickname)
{
	this->_cli_nickname = nickname;
}

void	Client::setUsername(std::string const &username)
{
	this->_cli_username = username;
}

void	Client::setRealname(std::string const &realname)
{
	this->_cli_realname = realname;
}

void	Client::setHostname(std::string const &hostname)
{
	this->_cli_hostname = hostname;
}

int	Client::getFd(void) const
{
	return (this->_cli_fd);
}

int	Client::getPort(void) const
{
	return (this->_cli_port);
}

bool Client::getRegistration() const
{
	return (this->_cli_registered);
}

std::string	Client::getNickname(void) const
{
	return (this->_cli_nickname);
}

std::string	Client::getUsername(void) const
{
	return (this->_cli_username);
}

std::string	Client::getRealname(void) const
{
	return (this->_cli_realname);
}

std::string	Client::getHostname(void) const
{
	return (this->_cli_hostname);
}

std::string Client::getPrefix(void) const
{
    std::string username = _cli_username.empty() ? "" : "!" + _cli_username;
    std::string hostname = _cli_hostname.empty() ? "" : "@" + _cli_hostname;

    return (_cli_nickname + username + hostname);
}
