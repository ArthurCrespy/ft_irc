/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_botLog.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:50:43 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/04 16:50:43 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

/**
	* @brief	Run the LOGBOT bot who register a user in the server
	* @param	int fd : The file descriptor of the client who sent the command
	* @param	std::string msg : The message to handle
*/
void Server::logBot(int fd, std::string const &msg)
{
	std::istringstream iss(msg);
	std::string command, password, nickname, username, realname;

	iss >> command >> password >> nickname >> username >> realname;

	if (command != "LOGBOT")
	{
		ft_send(fd, ERR_UNKNOWNCOMMAND(_client.at(fd)->getHostname(), command), 0);
		return ;
	}

	if (_client.at(fd)->getRegistration())
	{
		ft_send(fd, ERR_ALREADYREGISTERED(_client.at(fd)->getHostname()), 0);
		return ;
	}

	if (!password.empty() && password[0] == ':')
		password.erase(0, 1);

	if (password.empty() || nickname.empty() || username.empty() || realname.empty())
		ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getHostname(), command), 0);
	else if (password == _srv_password)
	{
		if (_user.count(nickname))
		{
			ft_send(fd, ERR_NICKNAMEINUSE(nickname), 0);
			return ;
		}
		_client.at(fd)->setNickname(nickname);
		_client.at(fd)->setUsername(username);
		_client.at(fd)->setRealname(realname);
		_client.at(fd)->setRegistration(true);
		_user.insert(std::make_pair(nickname, *_client.at(fd)));
		ft_send(fd, RPL_LBTREGISTER(_client.at(fd)->getNickname()), 0);
	}
	else
		ft_send(fd, ERR_PASSWDMISMATCH(_client.at(fd)->getHostname()), 0);
}
