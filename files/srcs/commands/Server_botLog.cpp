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
	std::string result;
	std::string command;
	std::string password;
	std::string nickname;
	std::string username;
	std::string realname;
	std::istringstream iss(msg);
	Client *client = _client.find(fd)->second;

	iss >> command >> password >> nickname >> username;
	std::getline(iss, realname);

	result = client->getUsername().empty() ? client->getHostname() : client->getUsername();

	if (command != "logbot")
		return (servSend(_srv_sock, fd, ERR_UNKNOWNCOMMAND(result, command)));

	if (client->getRegistration())
		return (servSend(-1, fd, ERR_ALREADYREGISTERED(result)));

	if (password[0] == ':')
		password.erase(0, 1);

	if (client->getIdentification() && (!password.empty() && nickname.empty() && username.empty() && realname.empty()))
	{
		if (password == _srv_password)
		{
			client->setRegistration(true);
			servSend(_srv_sock, fd, RPL_LBLOGGED(client->getNickname()));
		}
		else
			servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(result));
	}
	else if (password.empty() || nickname.empty() || username.empty() || realname.empty())
		servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(client->getHostname(), command));
	else if (password == _srv_password)
	{
		if (isClient(nickname) || nickname == "logbot" || nickname == "localhost")
			servSend(_srv_sock, fd, ERR_NICKNAMEINUSE(nickname, nickname));
		else
		{
			client->setNickname(nickname);
			client->setUsername(username);
			client->setRealname(realname);
			client->setIdentification(true);
			client->setRegistration(true);
			servSend(_srv_sock, fd, RPL_LBLOGGED(nickname));
			servSend(_srv_sock, fd, RPL_LBWELCOME(nickname, username, client->getHostname()));
		}
	}
	else
		servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(result));
}
