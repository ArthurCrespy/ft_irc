/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:17:29 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/03 00:36:50 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::setPort(int port)
{
	if (port < 1024 || port > 49151)
	{
		throw std::invalid_argument("Port must be between 1024 and 49151");
	}
	this->_srv_port = port;
}

void Server::setPort(std::string const &input)
{
	int port;

	try
	{
		port = ft_stoi(input);
	}
	catch (std::invalid_argument &e)
	{
		throw std::invalid_argument("Port must be a number");
	}
	if (port < 1024 || port > 49151)
	{
		throw std::invalid_argument("Port must be between 1024 and 49151");
	}

	this->_srv_port = port;
}

void Server::setPassword(std::string const &password)
{
	if (password.empty())
	{
		throw std::invalid_argument("Password cannot be empty");
	}
	this->_srv_password = password;
}

int	Server::getPort(void) const
{
	return (this->_srv_port);
}

std::string	Server::getPassword(void) const
{
	return (this->_srv_password);
}

int Server::getSock(void) const
{
	return (this->_srv_sock);
}

Channel &Server::getchannel(const std::string & name_channel)
{
	return (_channel.at(name_channel));
}
