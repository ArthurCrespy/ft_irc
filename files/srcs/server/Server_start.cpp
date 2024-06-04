/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_start.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:22:15 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/04 11:22:56 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"
#include "../../includes/ft_irc.h"
#include "../../includes/server/Server.hpp"
#include "../../includes/client/Client.hpp"
#include "../../includes/channel/User.hpp"
#include <deque>
#include <string>

/**
	*  @brief	Setup the server socket
	*  @details Make the socket to accept any addresses (even local duplicates ones) from IPv4 on nonblocking I/O mode,
	*  			bind the socket to the address, and listen on it
	*  @param	None
	*  @return	None
	*  @throw	std::runtime_error if the syscall socket(), setsockopt(), fcntl(), bind(), or listen() fails
*/
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
		throw std::runtime_error(e.what());
	}
	ft_print("Server listening on port " + ft_nbtos(this->getPort()), RUN);
}

/**
	*  @brief	Loop to poll the server socket and client sockets
	*  @param	None
	*  @return	None
	*  @throw	std::runtime_error if the syscall poll() or timeout fails
*/
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

		for (std::vector<pollfd>::iterator it = this->_poll.begin(); it != _poll.end(); ++it)
		{
			std::cout << "JE SUIS LQ DQLD " << it->fd << " AFHOFASF" << std::endl;
			if (it->revents == 0)
				continue ;

			else if (it->revents & POLLIN && it->fd == _srv_sock)
				servConnect();
			else if (it->revents & POLLIN)
				this->servReceive(it);
			else if (it->revents & POLLHUP)
				servClose(it->fd);
			break ;
		}
	}
}


/**
	*  @brief	Signal handler for SIGINT and SIGTERM
	*  @param	None
	*  @return	None
	*  @throw	std::runtime_error if the syscall sigaction() fails
*/
void Server::servSignal(void)
{
	_signal.sa_sigaction = ft_servSHandler;
	sigemptyset(&_signal.sa_mask);
	_signal.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT, &_signal, NULL) == -1
		|| sigaction(SIGTERM, &_signal, NULL) == -1)
		throw std::runtime_error("Syscall sigaction() Failed in servSignal: " + (std::string)std::strerror(errno));
}

/**
	*  @brief	Accept a new connection on the server socket
	*  @param	None
	*  @return	None
	*  @throw	std::runtime_error if the syscall accept() or getnameinfo() fails
*/
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

	std::string client_name = cli_name_in;
	client.ft_send(cli_fd, RPL_WELCOME(client_name), 0);

	_poll.push_back(cli_poll_in);
	_client.insert(std::make_pair(cli_fd, client));

	ft_print("Connection opened: " + (std::string)cli_name_in, LOG);
}

/**
	*  @brief	Receive data from a client socket and send it to the client. Close the connection if the client send disconnect message
	*  @param	int fd : file descriptor of the client socket
	*  @return	None
	*  @throw	std::runtime_error if the syscall recv() fails
*/
void Server::servReceive(std::vector<pollfd>::iterator &it)
{
	Client& client = _client.find(it->fd)->second;
	User				*user = client.findUserByFd(it->fd);
	std::cout << "DIT MOIS QUE CEST LA BONNE " << user << std::endl;
	long		bytes;
	char		buffer[1024];
	std::string	msg;

	memset(buffer, 0, 1024);
	while (1)
	{
		bytes = recv(it->fd, buffer, sizeof(buffer), 0);
		if (bytes == -1)
			throw std::runtime_error("Syscall recv() Failed in servReceive: " + std::string(std::strerror(errno)));
		if (bytes == 0)
		{
			servClose(it->fd);
			return ;
		}
		msg.append(buffer, bytes);
		if (msg.find("\r\n") != std::string::npos || msg.find('\n') != std::string::npos) // only \n for netcat ???
			break ;
		memset(buffer, 0, sizeof(buffer));
	}
	_client.at(it->fd).cliReceive(msg, it->fd, *this);
}

// todo: add the support of nc client
//       add the ctrl+d + ctrl+z logic

/**
	*  @brief	Close a client socket and remove it from the poll list
	*  @param	int fd : file descriptor of the client socket
	*  @return	None
	*  @throw	std::runtime_error if the syscall close() fails
*/
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


// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   Server_start.cpp                                   :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/05/14 16:22:15 by acrespy           #+#    #+#             */
// /*   Updated: 2024/05/30 14:57:21 by jdegluai         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../includes/ft_irc.h"

// /**
// 	*  @brief	Setup the server socket
// 	*  @details Make the socket to accept any addresses (even local duplicates ones) from IPv4 on nonblocking I/O mode,
// 	*  			bind the socket to the address, and listen on it
// 	*  @param	None
// 	*  @return	None
// 	*  @throw	std::runtime_error if the syscall socket(), setsockopt(), fcntl(), bind(), or listen() fails
// */
// void Server::servListen(void)
// {
// 	int				srv_opt;
// 	t_sock_addr_in	srv_sock_adrr;

// 	srv_opt = 1;
// 	srv_sock_adrr.sin_family = AF_INET;
// 	srv_sock_adrr.sin_port = htons(this->getPort());
// 	srv_sock_adrr.sin_addr.s_addr = INADDR_ANY;

// 	_srv_sock = socket(AF_INET, SOCK_STREAM, 0);
// 	if (_srv_sock == -1)
// 		throw std::runtime_error("Syscall socket() Failed in servListen: " + (std::string)std::strerror(errno));

// 	try
// 	{
// 		if (setsockopt(_srv_sock, SOL_SOCKET, SO_REUSEADDR, &srv_opt, sizeof(srv_opt)) == -1)
// 			throw std::runtime_error("Syscall setsockopt() Failed in servListen: " + (std::string) std::strerror(errno));

// 		if (fcntl(_srv_sock, F_SETFL, O_NONBLOCK) == -1)
// 			throw std::runtime_error("Syscall fcntl() Failed in servListen: " + (std::string) std::strerror(errno));

// 		if (bind(_srv_sock, (struct sockaddr *) &srv_sock_adrr, sizeof(srv_sock_adrr)) == -1)
// 			throw std::runtime_error("Syscall bind() Failed in servListen: " + (std::string) std::strerror(errno));

// 		if (listen(_srv_sock, SRV_MAX) == -1)
// 			throw std::runtime_error("Syscall listen() Failed in servListen: " + (std::string) std::strerror(errno));
// 	}
// 	catch (std::exception &e)
// 	{
// 		if (close(_srv_sock) == -1)
// 			throw std::runtime_error("Syscall close() Failed: " + (std::string)std::strerror(errno) + " after syscall failure in servListen");
// 		throw std::runtime_error(e.what());
// 	}
// 	ft_print("Server listening on port " + ft_nbtos(this->getPort()), RUN);
// }

// /**
// 	*  @brief	Loop to poll the server socket and client sockets
// 	*  @param	None
// 	*  @return	None
// 	*  @throw	std::runtime_error if the syscall poll() or timeout fails
// */
// void Server::servPoll(void)
// {
// 	int			srv_poll_ret;
// 	t_poll_fd	srv_poll;
// 	Client		client(_srv_sock, this->getPort(), "server");


// 	srv_poll.fd = _srv_sock;
// 	srv_poll.events = POLLIN;
// 	srv_poll.revents = 0;

// 	_poll.push_back(srv_poll);
// 	_client.insert(std::make_pair(srv_poll.fd, client));

// 	while (true)
// 	{
// 		std::cout << "TEST1" << std::endl;
// 		srv_poll_ret = poll(_poll.data(), _poll.size(), TIMEOUT);
// 		if (srv_poll_ret == -1)
// 		{
// 			std::cout << "TEST11" << std::endl;
// 			if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
// 			{
// 			std::cout << "TEST12" << std::endl;	
// 				continue ;
// 			}
// 			throw std::runtime_error("Syscall poll() Failed in servPoll: " + (std::string)std::strerror(errno));
// 		}
// 		if (srv_poll_ret == 0)
// 		{
// 		std::cout << "TEST13" << std::endl;	
// 			throw std::runtime_error("Syscall poll() timeout'd in servPoll after " + ft_nbtos(TIMEOUT) + " ms");
// 		}

// 		for (it_poll it = _poll.begin(); it != _poll.end(); ++it)
// 		{
// 			std::cout << "TEST14" << std::endl;
// 			if (it->revents == 0)
// 			{
// 				std::cout << "TEST15" << std::endl;
// 				continue ;
// 			}

// 			else if (it->revents & POLLIN && it->fd == _srv_sock)
// 			{
// 				std::cout << "TEST16" << std::endl;
// 				servConnect();
// 			}
// 			else if (it->revents & POLLIN)
// 			{
// 				std::cout << "TEST17" << std::endl;
// 				servReceive(it->fd);
// 			}
// 			else if (it->revents & POLLHUP)
// 			{
// 				std::cout << "TEST18" << std::endl;
// 				servClose(it->fd);
// 			}
// 			break ;
// 		}
// 	}
// }


// /**
// 	*  @brief	Signal handler for SIGINT and SIGTERM
// 	*  @param	None
// 	*  @return	None
// 	*  @throw	std::runtime_error if the syscall sigaction() fails
// */
// void Server::servSignal(void)
// {
// 	_signal.sa_sigaction = ft_servSHandler;
// 	sigemptyset(&_signal.sa_mask);
// 	_signal.sa_flags = SA_SIGINFO;

// 	if (sigaction(SIGINT, &_signal, NULL) == -1
// 		|| sigaction(SIGTERM, &_signal, NULL) == -1)
// 		throw std::runtime_error("Syscall sigaction() Failed in servSignal: " + (std::string)std::strerror(errno));
// }

// /**
// 	*  @brief	Accept a new connection on the server socket
// 	*  @param	None
// 	*  @return	None
// 	*  @throw	std::runtime_error if the syscall accept() or getnameinfo() fails
// */
// void Server::servConnect(void)
// {
// 	int				cli_fd;
// 	int 			cli_name_len;
// 	char			cli_name_in[NI_MAXHOST];
// 	t_sock_addr_in	cli_adrr_in;
// 	t_sock_len		cli_adrr_len;
// 	t_poll_fd		cli_poll_in;

// 	cli_adrr_len = sizeof(cli_adrr_in);
// 	cli_poll_in.fd = -1;
// 	cli_poll_in.events = POLLIN;
// 	cli_poll_in.revents = 0;

// 	cli_fd = accept(_srv_sock, (struct sockaddr *) &cli_adrr_in, &cli_adrr_len);
// 	if (cli_fd == -1)
// 	{
// 		if (errno == EWOULDBLOCK)
// 			return ;
// 		else
// 			throw std::runtime_error("Syscall accept() Failed in servConnect: " + (std::string)std::strerror(errno));
// 	}
// 	cli_poll_in.fd = cli_fd;

// 	cli_name_len = getnameinfo((struct sockaddr *) &cli_adrr_in, sizeof(cli_adrr_in), cli_name_in, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
// 	if (cli_name_len != 0)
// 		throw std::runtime_error("Syscall getnameinfo() Failed in servConnect: " + (std::string)std::strerror(errno));

// 	Client client(cli_fd, ntohs(cli_adrr_in.sin_port), cli_name_in);

// 	std::string client_name = cli_name_in;
// 	// send(cli_fd, RPL_WELCOME(client_name).c_str(), 1024, 0); // pas sur qu'il soit place a la bonne place mais c'est la reponse qu'attend le client
// 	send(cli_fd, RPL_WELCOME(client_name).c_str(), RPL_WELCOME(client_name).length(), 0);
	
// 	_poll.push_back(cli_poll_in);
// 	_client.insert(std::make_pair(cli_fd, client));

// 	ft_print("Connection opened: " + (std::string)cli_name_in, LOG);
// }

// /**
// 	*  @brief	Receive data from a client socket and send it to the client. Close the connection if the client send disconnect message
// 	*  @param	int fd : file descriptor of the client socket
// 	*  @return	None
// 	*  @throw	std::runtime_error if the syscall recv() fails
// */
// void Server::servReceive(int fd)
// {
// 	long		bytes;
// 	char		buffer[1024];
// 	std::string	msg;

// 	memset(buffer, 0, 1024);
// 	while (1)
// 	{
// 		bytes = recv(fd, buffer, sizeof(buffer), 0);
// 		if (bytes == -1)
// 			throw std::runtime_error("Syscall recv() Failed in servReceive: " + std::string(std::strerror(errno)));
// 		if (bytes == 0)
// 		{
// 			servClose(fd);
// 			return;
// 		}
// 		msg.append(buffer, bytes);
// 		if (msg.find("\r\n") != std::string::npos)
// 			break;
// 		memset(buffer, 0, sizeof(buffer));
// 	}
// 	_client.at(fd).cliReceive(msg, fd);
// }

// // todo: add the support of nc client
// //       add the ctrl+d + ctrl+z logic

// /**
// 	*  @brief	Close a client socket and remove it from the poll list
// 	*  @param	int fd : file descriptor of the client socket
// 	*  @return	None
// 	*  @throw	std::runtime_error if the syscall close() fails
// */
// void Server::servClose(int fd)
// {
// 	if (close(fd) == -1)
// 		throw std::runtime_error("Syscall close() Failed in servClose: " + (std::string)std::strerror(errno));

// 	ft_print("Connection closed: " + _client.at(fd).getHostname(), LOG);

// 	for (it_poll it = _poll.begin(); it != _poll.end(); it++)
// 	{
// 		if (it->fd == fd)
// 		{
// 			_poll.erase(it);
// 			break ;
// 		}
// 	}
// 	_client.erase(_client.find(fd));
// }
