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
	std::string username;
	std::string optname;
	std::string hostname;

	std::istringstream iss(msg);
	iss >> username >> optname >> hostname;

	std::string realname;
	std::getline(iss, realname);

	while (realname[0] == ' ' || realname[0] == ':')
		realname.erase(0, 1);
	if (username.empty() || realname.empty())
		servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "USER"));
	else
	{
		_client.find(fd)->second->setUsername(username);
		_client.find(fd)->second->setRealname(realname);
		_client.find(fd)->second->setHostname(hostname);
		_client.find(fd)->second->setIdentification(true);
		servSend(_srv_sock, fd, RPL_LBWELCOME(_client.at(fd)->getNickname(), _client.at(fd)->getUsername(), _client.at(fd)->getHostname()));
	}
}
