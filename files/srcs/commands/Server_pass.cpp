/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_pass.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:57:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/17 11:57:57 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::pass(int fd, std::string const &msg)
{
	std::string password;
	std::istringstream iss(msg);
	std::getline(iss, password);
	Client *client = _client.at(fd);

	if (password[0] == ':')
		password.erase(0, 1);
	if (password.empty())
		servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
	else if (client->getLogged())
		servSend(_srv_sock, fd, ERR_ALREADYREGISTERED(client->getNickname()));
	else if (password == _srv_password)
	{
		if (client->getUser())
		{
			client->setLogged(true);
			servSend(_srv_sock, fd, RPL_WELCOME(client->getNickname(), client->getUsername(), client->getHostname()));
		}
		client->setPassword(true);
	}
	else
		servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(client->getNickname()));
}
