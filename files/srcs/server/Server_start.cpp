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

#include "../../includes/ft_irc.h"

void Server::servListen(void)
{
	int				srv_opt;
	t_sockaddr_in	srv_sock_adrr;

	srv_opt = 1;
	srv_sock_adrr.sin_family = AF_INET;
	srv_sock_adrr.sin_port = htons(this->getPort());
	srv_sock_adrr.sin_addr.s_addr = INADDR_ANY;

	_srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_srv_sock == -1)
		throw std::runtime_error("Syscall socket() Failed in servListen: " + (std::string)std::strerror(errno));

	// todo: from this line close the socket when exeption is thrown

	if (setsockopt(_srv_sock, SOL_SOCKET, SO_REUSEADDR, &srv_opt, sizeof(srv_opt)) == -1)
		throw std::runtime_error("Syscall setsockopt() Failed in servListen: " + (std::string)std::strerror(errno));

	if (fcntl(_srv_sock, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Syscall fcntl() Failed in servListen: " + (std::string)std::strerror(errno));

	if (bind(_srv_sock, (struct sockaddr *) &srv_sock_adrr, sizeof(srv_sock_adrr)) == -1)
		throw std::runtime_error("Syscall bind() Failed in servListen: " + (std::string)std::strerror(errno));

	if (listen(_srv_sock, SRV_MAX) == -1)
		throw std::runtime_error("Syscall listen() Failed in servListen: " + (std::string)std::strerror(errno));

	ft_print("Server listening on port " + ft_nbtos(this->getPort()), RUN);
}

void Server::servPoll(void)
{
	int			poll_ret;
	t_pollfd	srv_poll;

	srv_poll.fd = _srv_sock;
	srv_poll.events = POLLIN;
	srv_poll.revents = 0;
	_pclimap.insert(std::pair<t_pollfd *, Client>(&srv_poll, Client(_srv_sock, this->getPort(), "server")));

	while (true)
	{
		poll_ret = poll(_pclimap.begin()->first, _pclimap.size(), TIMEOUT);
		if (poll_ret == -1)
			throw std::runtime_error("Syscall poll() Failed in servPoll: " + (std::string)std::strerror(errno));
		if (poll_ret == 0)
			throw std::runtime_error("Syscall poll() timeout'd in servPoll after " + ft_nbtos(TIMEOUT) + " ms");

		for (it_pclimap it = _pclimap.begin(); it != _pclimap.end(); ++it)
		{
			if (it->first->revents == 0)
				continue ;

			else if (it->first->revents & POLLIN && it->first->fd == _srv_sock)
				servConnect();

			else if (it->first->revents & POLLIN)
				servReceive(it->first);
			else if (it->first->revents & POLLHUP)
				servClose(it->first);
			else
				throw std::runtime_error("Syscall poll() Failed in servPoll: " + (std::string)std::strerror(errno));
			break ;
		}
	}

}

void Server::servConnect(void)
{
	int				cli_fd;
	int 			cli_name_len;
	char			cli_name_in[NI_MAXHOST];
	t_sockaddr_in	cli_adrr_in;
	t_socklen		cli_adrr_len;
	t_pollfd		cli_poll_in;

	cli_adrr_len = sizeof(cli_adrr_in);
	cli_fd = accept(_srv_sock, (struct sockaddr *) &cli_adrr_in, &cli_adrr_len);
	if (cli_fd == -1)
		throw std::runtime_error("Syscall accept() Failed in servConnect: " + (std::string)std::strerror(errno));

	cli_poll_in.fd = cli_fd;
	cli_poll_in.events = POLLIN;
	cli_poll_in.revents = 0;

	cli_name_len = getnameinfo((struct sockaddr *) &cli_adrr_in, sizeof(cli_adrr_in), cli_name_in, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	if (cli_name_len != 0)
		throw std::runtime_error("Syscall getnameinfo() Failed in servConnect: " + (std::string)std::strerror(errno));

	_pclimap.insert(std::pair<t_pollfd *, Client>(&cli_poll_in, Client(cli_fd, ntohs(cli_adrr_in.sin_port), cli_name_in)));

	ft_print("Connection opened: " + (std::string)cli_name_in, LOG);
}

void Server::servReceive(t_pollfd *pollfd)
{
	size_t		bytes;
	char		buffer[1024];
	std::string	msg;

	while (!strstr(buffer, "\r\n"))
	{
		bytes = recv(pollfd->fd, buffer, 1024, 0); // Socket operation on non-socket
//		bytes = recv(_srv_sock, buffer, 1024, 0); // Socket is not connected
		if (bytes == -1)
			throw std::runtime_error("Syscall recv() Failed in servReceive: " + (std::string) std::strerror(errno));
		if (bytes == 0)
			servClose(pollfd);
		else
			msg += buffer;
	}
	_pclimap[pollfd].cliReceive(msg);
}

void Server::servClose(t_pollfd *pollfd)
{
	if (close(pollfd->fd) == -1)
		throw std::runtime_error("Syscall close() Failed in servClose: " + (std::string)std::strerror(errno));

	ft_print("Connection closed: fd" + ft_nbtos(pollfd->fd), LOG);

	_pclimap.erase(_pclimap.find(pollfd));
}
