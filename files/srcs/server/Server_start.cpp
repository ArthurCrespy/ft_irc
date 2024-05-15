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

void Server::servSocket(void)
{
	_opt_srv = 1;
	_sock_adrr_srv.sin_family = AF_INET;
	_sock_adrr_srv.sin_port = htons(this->getPort());
	_sock_adrr_srv.sin_addr.s_addr = INADDR_ANY;

	_sock_srv = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock_srv == -1)
		throw std::runtime_error("Syscall socket Failed in servSocket: " + (std::string)std::strerror(errno));

	if (setsockopt(_sock_srv, SOL_SOCKET, SO_REUSEADDR, &_opt_srv, sizeof(_opt_srv)) == -1)
		throw std::runtime_error("Syscall setsockopt Failed in servSocket: " + (std::string)std::strerror(errno));

	if (bind(_sock_srv, (struct sockaddr *) &_sock_adrr_srv, sizeof(_sock_adrr_srv)) == -1)
		throw std::runtime_error("Syscall bind Failed in servSocket: " + (std::string)std::strerror(errno));

	if (listen(_sock_srv, 10) == -1)
		throw std::runtime_error("Syscall listen Failed in servSocket: " + (std::string)std::strerror(errno));

	ft_print("Server listening on port " + ft_nbtos(this->getPort()), RUN);
}

void Server::servPoll(void)
{
	_poll_srv.fd = _sock_srv;
	_poll_srv.events = POLLIN;
	_poll_srv.revents = 0;

	_poll_fds.push_back(_poll_srv);

	while (true)
	{
		if (poll(_poll_fds.data(), _poll_fds.size(), -1) == -1)
			throw std::runtime_error("Syscall poll Failed in servPoll: " + (std::string)std::strerror(errno));

		for (it_pollfds it = _poll_fds.begin(); it != _poll_fds.end(); it++)
		{
			if (it->revents & POLLIN && it->fd == _sock_srv)
			{
				ft_print("Recived POLLIN (server socket)", NONE);
				servAccept();
			}
			else if (it->revents & POLLIN)
				ft_print("Recived POLLIN", NONE); // Handle input
			else if (it->revents & POLLHUP)
			{
				ft_print("Recived POLLHUP", NONE);
			}
			else if (it->revents & POLLERR)
				ft_print("Recived POLLERR", NONE); // Handle error
			else if (it->revents == 0)
				ft_print("Recived 0", NONE);
			else
				throw std::runtime_error("Unknown event in servPoll: " + ft_nbtos(it->revents));
		}
	}

}

void Server::servAccept(void)
{
	int				fd_in;
	int 			name_len;
	char			name_in[NI_MAXHOST];
	t_sockaddr_in	adrr_in;
	t_socklen		adrr_len;
	t_pollfd		poll_in;


	fd_in = accept(_sock_srv, (struct sockaddr *) &adrr_in, &adrr_len);
	if (fd_in == -1)
		throw std::runtime_error("Syscall accept Failed in servAccept: " + (std::string)std::strerror(errno));

	poll_in.fd = fd_in;
	poll_in.events = POLLIN;
	poll_in.revents = 0;
	_poll_fds.push_back(poll_in);

	name_len = getnameinfo((struct sockaddr *) &adrr_in, sizeof(adrr_in), name_in, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	if (name_len == -1)
		throw std::runtime_error("Syscall getnameinfo Failed in servAccept: " + (std::string)std::strerror(errno));

	// Add a new client to the list

	ft_print("Connection opened: " + (std::string)name_in, LOG);
}

void Server::start(void)
{
	this->servSocket();
	this->servPoll();
}
