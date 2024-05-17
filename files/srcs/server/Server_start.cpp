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
	t_sock_addr_in	srv_sock_adrr;

	srv_opt = 1;
	srv_sock_adrr.sin_family = AF_INET;
	srv_sock_adrr.sin_port = htons(this->getPort());
	srv_sock_adrr.sin_addr.s_addr = INADDR_ANY;

	_srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_srv_sock == -1)
		throw std::runtime_error("Syscall socket() Failed in servListen: " + (std::string)std::strerror(errno));

	try
	{
		if (setsockopt(_srv_sock, SOL_SOCKET, SO_REUSEADDR, &srv_opt, sizeof(srv_opt)) == -1)
			throw std::runtime_error("Syscall setsockopt() Failed in servListen: " + (std::string) std::strerror(errno));

		if (fcntl(_srv_sock, F_SETFL, O_NONBLOCK) == -1)
			throw std::runtime_error("Syscall fcntl() Failed in servListen: " + (std::string) std::strerror(errno));

		if (bind(_srv_sock, (struct sockaddr *) &srv_sock_adrr, sizeof(srv_sock_adrr)) == -1)
			throw std::runtime_error("Syscall bind() Failed in servListen: " + (std::string) std::strerror(errno));

		if (listen(_srv_sock, SRV_MAX) == -1)
			throw std::runtime_error("Syscall listen() Failed in servListen: " + (std::string) std::strerror(errno));
	}
	catch (std::exception &e)
	{
		if (close(_srv_sock) == -1)
			throw std::runtime_error("Syscall close() Failed: " + (std::string)std::strerror(errno) + " after syscall failure in servListen");
		throw (e);
	}
	ft_print("Server listening on port " + ft_nbtos(this->getPort()), RUN);
}

void Server::servPoll(void)
{
	int			srv_poll_ret;
	t_poll_fd	srv_poll;
	Client		client(_srv_sock, this->getPort(), "server");


	srv_poll.fd = _srv_sock;
	srv_poll.events = POLLIN;
	srv_poll.revents = 0;

	_poll.push_back(srv_poll);
	_client.insert(std::make_pair(srv_poll.fd, client));

	while (true)
	{
		srv_poll_ret = poll(_poll.data(), _poll.size(), TIMEOUT);
		if (srv_poll_ret == -1)
		{
            if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
                continue ;
			throw std::runtime_error("Syscall poll() Failed in servPoll: " + (std::string)std::strerror(errno));
		}
		if (srv_poll_ret == 0)
			throw std::runtime_error("Syscall poll() timeout'd in servPoll after " + ft_nbtos(TIMEOUT) + " ms");

		for (it_poll it = _poll.begin(); it != _poll.end(); ++it)
		{
			if (it->revents == 0)
				continue ;

			else if (it->revents & POLLIN && it->fd == _srv_sock)
				servConnect();

			else if (it->revents & POLLIN)
				servReceive(it->fd);
			else if (it->revents & POLLHUP)
				servClose(it->fd);
			break ;
		}
	}
}

void Server::servConnect(void)
{
	int				cli_fd;
	int 			cli_name_len;
	char			cli_name_in[NI_MAXHOST];
	t_sock_addr_in	cli_adrr_in;
	t_sock_len		cli_adrr_len;
	t_poll_fd		cli_poll_in;

	cli_adrr_len = sizeof(cli_adrr_in);
	cli_poll_in.fd = -1;
	cli_poll_in.events = POLLIN;
	cli_poll_in.revents = 0;

	cli_fd = accept(_srv_sock, (struct sockaddr *) &cli_adrr_in, &cli_adrr_len);
	if (cli_fd == -1)
	{
		if (errno == EWOULDBLOCK)
			return ;
		else
			throw std::runtime_error("Syscall accept() Failed in servConnect: " + (std::string)std::strerror(errno));
	}
	cli_poll_in.fd = cli_fd;

	cli_name_len = getnameinfo((struct sockaddr *) &cli_adrr_in, sizeof(cli_adrr_in), cli_name_in, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	if (cli_name_len != 0)
		throw std::runtime_error("Syscall getnameinfo() Failed in servConnect: " + (std::string)std::strerror(errno));

	Client client(cli_fd, ntohs(cli_adrr_in.sin_port), cli_name_in);

	_poll.push_back(cli_poll_in);
	_client.insert(std::make_pair(cli_fd, client));

	ft_print("Connection opened: " + (std::string)cli_name_in, LOG);
}

void Server::servReceive(int fd)
{
	long		bytes;
	char		buffer[1024];
	std::string	msg;

	memset(buffer, 0, 1024);
	while (!strstr(buffer, "\r\n"))
	{
		bytes = recv(fd, buffer, 1024, 0);
		if (bytes == -1)
			throw std::runtime_error("Syscall recv() Failed in servReceive: " + (std::string) std::strerror(errno));
		if (bytes == 0)
			break ;
		else
			msg.append(buffer, bytes);
	}

	if (msg.empty())
	{
		servClose(fd);
		return ;
	}
	else
		_client.at(fd).cliReceive(msg);
}

void Server::servClose(int fd)
{
	if (close(fd) == -1)
		throw std::runtime_error("Syscall close() Failed in servClose: " + (std::string)std::strerror(errno));

	ft_print("Connection closed: " + _client.at(fd).getHostname(), LOG);

	for (it_poll it = _poll.begin(); it != _poll.end(); it++)
	{
		if (it->fd == fd)
		{
			_poll.erase(it);
			break ;
		}
	}
	_client.erase(_client.find(fd));
}
