/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_nick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:06:03 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/15 11:20:50 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::nick(int fd, std::string const &msg)
{
	std::string nickname;
	std::istringstream iss(msg);
	Client *client = _client.find(fd)->second;

	iss >> nickname;
	if (nickname[0] == ':')
		nickname.erase(0, 1);
	if (nickname.empty())
		servSend(_srv_sock, fd, ERR_NONICKNAMEGIVEN(client->getNickname()));
	if (nickname == client->getNickname())
		return ;
	else if (isClient(nickname) || nickname == "logbot" || nickname == "localhost")
		servSend(_srv_sock, fd, ERR_NICKNAMEINUSE(nickname, nickname));
	else if (!client->getRegistration())
	{
		client->setNickname(nickname);
		client->setIdentification(true);
	}
	else
	{
		std::string old_nickname = client->getNickname();
		client->setNickname(nickname);
		servSend(_srv_sock, fd, RPL_NICK(old_nickname, nickname));
	}
}
