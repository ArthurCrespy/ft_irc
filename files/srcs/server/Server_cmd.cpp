/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:54:01 by abinet            #+#    #+#             */
/*   Updated: 2024/06/09 13:55:36 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::servCommand(int fd, std::string const &msg)
{
	ft_print("Client received: " + msg, INFO);

	std::istringstream iss(msg);
	std::string command;
	iss >> command;

	std::string remaining;
	std::getline(iss, remaining);

	if (command == "PING" || command == "/ping")
		return (servSend(_srv_sock, fd, RPL_PONG(_client.at(fd)->getNickname())));
	if ((command == "PRIVMSG" || command == "/msg") && remaining.find("LOGBOT") == 1)
		return (logBot(fd, remaining));

	if (!_client.at(fd)->getRegistration())
		return (servSend(_srv_sock, fd, ERR_NOLOGIN(_client.at(fd)->getHostname())));

	if ((remaining.empty() || remaining == "\r\n"))
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), command)));


	if (command == "PRIVMSG" || command == "/msg")
		msgSend(fd, remaining);
	else if (command == "JOIN" || command == "/join")
		{} // join(remaining, fd, server);
	else if (command == "KICK" || command == "/kick")
		kick(fd, remaining);
}
