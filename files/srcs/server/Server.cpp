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

Server::Server(void) : _srv_sock(-1), _srv_port(6667), _srv_password("default"), _signal()
{
	ft_print("Server void constructor called, using default values", WARN);
	ft_print("Port: " + ft_nbtos(this->getPort()), INFO);
	ft_print("Password: " + this->getPassword(), INFO);
	ft_print("Max simultaneous connections: " + ft_nbtos(SRV_MAX), INFO);
}

Server::Server(int argc, char **argv) : _srv_sock(-1), _srv_port(-1), _signal()
{
	servSetup(argc, argv);

	ft_print("Port: " + ft_nbtos(this->getPort()), INFO);
	ft_print("Password: " + this->getPassword(), INFO);
	ft_print("Max simultaneous connections: " + ft_nbtos(SRV_MAX), INFO);
}

Server::Server(int port, std::string const &password) : _srv_sock(-1), _srv_port(-1), _signal()
{
	setPort(port);
	setPassword(password);

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
	while (!_client.empty())
	{
		if (_client.begin()->second->getFd() != -1)
			if (close(_client.begin()->second->getFd()) == -1)
				throw std::runtime_error("Syscall close() _client Failed in ~Server: " + (std::string)std::strerror(errno));
		delete (_client.begin()->second);
		_client.erase(_client.begin());
	}
	_channel.clear();
	_client.clear();
	_poll.clear();

	if (_srv_sock != -1)
		close(_srv_sock);
	_srv_sock = -1;

	ft_print("Server stopped", STOP);
}

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->_srv_sock = rhs._srv_sock;
		this->_srv_port = rhs._srv_port;
		this->_srv_password = rhs._srv_password;
		this->_poll = rhs._poll;
		this->_client = rhs._client;
		this->_channel = rhs._channel;
		this->_signal = rhs._signal;
	}
	return (*this);
}

void Server::servStart(void)
{
	servSignal();
	servListen();
	servPoll();
}
