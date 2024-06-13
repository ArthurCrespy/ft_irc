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
	std::string result;
	std::string nickname;
	std::istringstream iss(msg);
	Client *client = _client.find(fd)->second;

	iss >> nickname;
	result = client->getUsername().empty() ? client->getHostname() : client->getUsername();
	if (nickname[0] == ':')
		nickname.erase(0, 1);
	if (nickname.empty())
		servSend(_srv_sock, fd, ERR_NONICKNAMEGIVEN(_client.at(fd)->getNickname()));
	else if (isClient(nickname))
		servSend(_srv_sock, fd, ERR_NICKNAMEINUSE(result, nickname));
	else if (!client->getRegistration())
		client->setNickname(nickname);
	else
	{
		std::string old_nickname = client->getNickname();
		client->setNickname(nickname);
		servSend(_srv_sock, fd, RPL_NICK(old_nickname, nickname));
	}
}
