/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_close.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:35:28 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 11:35:28 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

/**
	*  @brief	Close a client socket and remove it from the poll list
	*  @param	int fd : file descriptor of the client socket
	*  @return	None
	*  @throw	std::runtime_error if the syscall close() fails
*/
void Server::servClose(int fd)
{
	std::string nickname;

	if (close(fd) == -1)
		throw std::runtime_error("Syscall close() Failed in servClose: " + (std::string)std::strerror(errno));
	_client.at(fd)->setFd(-1);

	nickname = _client.at(fd)->getNickname();

	ft_print("Connection closed: " + _client.at(fd)->getHostname(), LOG);

	for (it_poll it = _poll.begin(); it != _poll.end(); it++)
	{
		if (it->fd == fd)
		{
			_poll.erase(it);
			break ;
		}
	}
	delete (_client.at(fd));
	_client.erase(_client.find(fd));
	if (_user.find(nickname) != _user.end())
		_user.erase(_user.find(nickname));
}
