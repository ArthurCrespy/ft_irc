/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:30:23 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/14 14:30:23 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

Server::Server(void) : _port(6667), _password("default"), _srv_sock(-1), _srv_opt(1), _srv_sock_adrr(), _srv_poll()
{
	ft_print("Server void constructor called, using default values", WARN);
	ft_print("Port: " + ft_nbtos(this->getPort()), INFO);
	ft_print("Password: " + this->getPassword(), INFO);
	ft_print("Max simultaneous connections: " + ft_nbtos(SRV_MAX), INFO);
}

Server::Server(int argc, char **argv) : _port(-1), _srv_sock(-1), _srv_opt(1), _srv_sock_adrr(), _srv_poll()
{
	this->servSetup(argc, argv);

	ft_print("Port: " + ft_nbtos(this->getPort()), INFO);
	ft_print("Password: " + this->getPassword(), INFO);
	ft_print("Max simultaneous connections: " + ft_nbtos(SRV_MAX), INFO);
}

Server::Server(int port, std::string const &password) : _port(-1), _srv_sock(-1), _srv_opt(1), _srv_sock_adrr(), _srv_poll()
{
	this->setPort(port);
	this->setPassword(password);

	ft_print("Port: " + ft_nbtos(this->getPort()), INFO);
	ft_print("Password: " + this->getPassword(), INFO);
	ft_print("Max simultaneous connections: " + ft_nbtos(SRV_MAX), INFO);
}

Server::Server(Server const &src)
{
	*this = src;
}

Server::~Server(void)
{
	ft_print("Server stopped", STOP);
}

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->_port = rhs._port;
		this->_password = rhs._password;
	}
	return (*this);
}

void Server::servStart(void)
{
	this->servListen();
	this->servPoll();
}
