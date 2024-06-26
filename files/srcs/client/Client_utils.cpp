/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:53:14 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/03 21:53:02 by abinet           ###   ########.fr       */
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

void	Client::setPassword(bool pwd)
{
	this ->_cli_p = pwd;
}
void	Client::setNick(bool nick)
{
	this ->_cli_n = nick;
}

void	Client::setUser(bool user)
{
	this ->_cli_u = user;
}

void	Client::setLogged(bool logged)
{
	this ->_cli_l = logged;
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

bool	Client::getPassword() const
{
	return (this->_cli_p);
}

bool	Client::getNick() const
{
	return (this->_cli_n);
}

bool	Client::getUser() const
{
	return (this->_cli_u);
}

bool	Client::getLogged() const
{
	return (this->_cli_l);
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

	if (_cli_nickname.empty() && _cli_username.empty())
		return (_cli_hostname);
	else
		return (_cli_nickname + username + hostname);
}
