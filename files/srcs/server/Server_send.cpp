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

void Server::ft_send(int fd, std::string const &msg, int flags)
{
	std::string str = ":" + _client.at(fd)->getPrefix() + " " + msg + "\r\n";

	if (send(fd, str.c_str(), ft_strlen(str), flags) == -1)
		throw std::runtime_error("Syscall send() Failed in send: " + std::string(std::strerror(errno)));
}
