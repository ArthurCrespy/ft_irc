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

	if (command != "logbot")
	{
		servSend(_srv_sock, fd, ERR_UNKNOWNCOMMAND(_client.at(fd)->getHostname(), command));
		return ;
	}

	if (_client.at(fd)->getRegistration())
	{
		servSend(-1, fd, ERR_ALREADYREGISTERED(_client.at(fd)->getHostname()));
		return ;
	}

	if (!password.empty() && password[0] == ':')
		password.erase(0, 1);

	if (!password.empty() && nickname.empty() && username.empty() && realname.empty() && _client.at(fd)->getIdentification())
	{
		if (password == _srv_password)
		{
			_client.at(fd)->setRegistration(true);
			servSend(_srv_sock, fd, RPL_LBLOGGED(_client.at(fd)->getNickname()));
		}
		else
			servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(_client.at(fd)->getHostname()));
	}
	else if (password.empty() || nickname.empty() || username.empty() || realname.empty())
		servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getHostname(), command));
	else if (password == _srv_password)
	{
		if (_user.count(nickname) || nickname == "logbot" || nickname == "localhost")
			servSend(_srv_sock, fd, ERR_NICKNAMEINUSE(nickname));
		else
		{
			_client.at(fd)->setNickname(nickname);
			_client.at(fd)->setUsername(username);
			_client.at(fd)->setRealname(realname);
			_client.at(fd)->setIdentification(true);
			_client.at(fd)->setRegistration(true);
			_user.insert(std::make_pair(nickname, *_client.at(fd)));
			servSend(_srv_sock, fd, RPL_LBLOGGED(_client.at(fd)->getNickname()));
			servSend(_srv_sock, fd, RPL_LBWELCOME(_client.at(fd)->getNickname(), _client.at(fd)->getUsername(), _client.at(fd)->getHostname()));
		}
	}
	else
		servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(_client.at(fd)->getHostname()));
}
