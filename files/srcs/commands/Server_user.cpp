/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_user.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:07:52 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/13 16:07:52 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::user(int fd, std::string const &msg)
{
	std::string optname;
	std::string username;
	std::string hostname;
	std::string realname;
	std::string nickname;
	std::istringstream iss(msg);
	Client *client = _client.find(fd)->second;

	iss >> username >> optname >> hostname;
	std::getline(iss, realname);

	nickname = client->getNickname();
	while (realname[0] == ' ' || realname[0] == ':')
		realname.erase(0, 1);
	if (username.empty() || realname.empty())
		servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(nickname, "USER"));
	else if (client->getLogged())
		servSend(_srv_sock, fd, ERR_ALREADYREGISTERED(nickname));
	else if (!client->getNick())
		servSend(_srv_sock, fd, ERR_NOTREGISTERED(client->getHostname()));
	else
	{
		client->setUsername(username);
		client->setRealname(realname);
		client->setUser(true);
		if (client->getPassword())
			client->setLogged(true);
		servSend(_srv_sock, fd, RPL_WELCOME(nickname, username, client->getHostname()));
	}
}
