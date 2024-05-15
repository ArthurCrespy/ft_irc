/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_start.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:22:15 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/14 16:22:15 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../includes/ft_irc.h"

void Server::servListen(void)
{
	_srv_opt = 1;
	_srv_sock_adrr.sin_family = AF_INET;
	_srv_sock_adrr.sin_port = htons(this->getPort());
	_srv_sock_adrr.sin_addr.s_addr = INADDR_ANY;

	_srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_srv_sock == -1)
		throw std::runtime_error("Syscall socket() Failed in servListen: " + (std::string)std::strerror(errno));

	// todo: from this line close the socket when exeption is thrown

	if (setsockopt(_srv_sock, SOL_SOCKET, SO_REUSEADDR, &_srv_opt, sizeof(_srv_opt)) == -1)
		throw std::runtime_error("Syscall setsockopt() Failed in servListen: " + (std::string)std::strerror(errno));

	if (fcntl(_srv_sock, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Syscall fcntl() Failed in servListen: " + (std::string)std::strerror(errno));

	if (bind(_srv_sock, (struct sockaddr *) &_srv_sock_adrr, sizeof(_srv_sock_adrr)) == -1)
		throw std::runtime_error("Syscall bind() Failed in servListen: " + (std::string)std::strerror(errno));

	if (listen(_srv_sock, SRV_MAX) == -1)
		throw std::runtime_error("Syscall listen() Failed in servListen: " + (std::string)std::strerror(errno));

	ft_print("Server listening on port " + ft_nbtos(this->getPort()), RUN);
}

void Server::servPoll(void)
{
	int poll_ret;

	_srv_poll.fd = _srv_sock;
	_srv_poll.events = POLLIN;
	_srv_poll.revents = 0;
	_poll_fds.push_back(_srv_poll);

	while (true)
	{
		poll_ret = poll(_poll_fds.data(), _poll_fds.size(), (3 * 60 * 1000)); // timeout 3 minutes
		if (poll_ret == -1)
			throw std::runtime_error("Syscall poll() Failed in servPoll: " + (std::string)std::strerror(errno));
		if (poll_ret == 0)
			throw std::runtime_error("Syscall poll() timeout in servPoll: " + ft_nbtos(poll_ret));

		for (it_pollfds it = _poll_fds.begin(); it != _poll_fds.end(); ++it)
		{
			if (it->revents == 0)
				continue ;

			else if (it->revents & POLLIN && it->fd == _srv_sock)
				servConnect();
			else if (it->revents & POLLIN)
				servReceive(it->fd);
			else if (it->revents & POLLHUP)
				servClose(it->fd);
			else
				throw std::runtime_error("Syscall poll() Failed in servPoll: " + (std::string)std::strerror(errno));
			break ;
		}
	}

}

void Server::servReceive(int fd)
{
	char buffer[1024];
	int bytes;

	bytes = recv(fd, buffer, 1024, 0);
	if (bytes == -1)
		throw std::runtime_error("Syscall recv() Failed in servPoll: " + (std::string)std::strerror(errno));
	if (bytes == 0)
		servClose(fd);
	else
		ft_print("Recived: " + (std::string)buffer, INFO);
}

void Server::servConnect(void)
{
	int				fd_in;
	int 			name_len;
	char			name_in[NI_MAXHOST];
	t_sockaddr_in	adrr_in;
	t_socklen		adrr_len;
	t_pollfd		poll_in;

	adrr_len = sizeof(adrr_in);
	fd_in = accept(_srv_sock, (struct sockaddr *) &adrr_in, &adrr_len);
	if (fd_in == -1)
		throw std::runtime_error("Syscall accept() Failed in servConnect: " + (std::string)std::strerror(errno));

	poll_in.fd = fd_in;
	poll_in.events = POLLIN;
	poll_in.revents = 0;
	_poll_fds.push_back(poll_in);

	name_len = getnameinfo((struct sockaddr *) &adrr_in, sizeof(adrr_in), name_in, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	if (name_len != 0)
		throw std::runtime_error("Syscall getnameinfo() Failed in servConnect: " + (std::string)std::strerror(errno));

	// todo: add a new client to the list

	ft_print("Connection opened: " + (std::string)name_in, LOG);
}

void Server::servClose(int fd)
{
	if (close(fd) == -1)
		throw std::runtime_error("Syscall close() Failed in servClose: " + (std::string)std::strerror(errno));

	for (it_pollfds it = _poll_fds.begin(); it != _poll_fds.end(); it++)
	{
		if (it->fd == fd)
		{
			ft_print("Connection closed: <unknown host>", LOG); // todo: get the host name (maybe stored in the client list)
			_poll_fds.erase(it);
			break;
		}
	}
}

void Server::servStart(void)
{
	this->servListen();
	this->servPoll();
}
