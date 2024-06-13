/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_nick.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:06:03 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/13 16:06:03 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::nick(int fd, std::string const &msg)
{
	std::string nickname;
	std::istringstream iss(msg);

	iss >> nickname;
	if (nickname[0] == ':')
		nickname.erase(0, 1);
	if (nickname.empty())
		servSend(_srv_sock, fd, ERR_NONICKNAMEGIVEN(_client.at(fd)->getNickname()));
	else if (_user.find(nickname) != _user.end())
		servSend(_srv_sock, fd, ERR_NICKNAMEINUSE(nickname));
	else if (!_client.find(fd)->second->getRegistration())
		_client.find(fd)->second->setNickname(nickname);
	else
	{
		std::string old_nickname = _client.find(fd)->second->getNickname();
		_client.find(fd)->second->setNickname(nickname);
		servSend(_srv_sock, fd, RPL_NICK(old_nickname, nickname));
	}
}
