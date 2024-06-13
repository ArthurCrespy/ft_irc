/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_send.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:54:05 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/04 16:54:05 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::servSend(int fd_src, int fd_dest, std::string const &msg)
{
	std::string str;

	if (fd_src == -1)
		str = ":logbot!localhost@localhost " + msg + "\r\n";
	else
		str = ":" + _client.at(fd_src)->getPrefix() + " " + msg + "\r\n";

	if (send(fd_dest, str.c_str(), ft_strlen(str), 0) == -1)
		throw std::runtime_error("Syscall send() Failed in send: " + std::string(std::strerror(errno)));
}
