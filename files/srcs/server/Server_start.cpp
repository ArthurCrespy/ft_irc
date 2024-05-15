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

void Server::serv_poll(void)
{

}

void Server::start(void)
{
	this->serv_poll();
	this->servSocket();
}
