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

void Server::serv_socket(void)
{
	_opt = 1;
	_sock_adrr.sin_family = AF_INET;
	_sock_adrr.sin_port = htons(this->getPort());
	_sock_adrr.sin_addr.s_addr = INADDR_ANY;

	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == -1)
		throw std::runtime_error("Socket creation failed");

	if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)) == -1)
		throw std::runtime_error("Setsockopt failed");

	if (bind(_sock, (struct sockaddr *) &_sock_adrr, sizeof(_sock_adrr)) == -1)
		throw std::runtime_error("Bind failed");

	if (listen(_sock, 10) == -1)
		throw std::runtime_error("Listen failed");

	ft_print("Server listening on port " + ft_nbtos(this->getPort()), RUN);
}

void Server::serv_poll(void)
{

}

void Server::start(void)
{
	this->serv_socket();
	this->serv_poll();
}
