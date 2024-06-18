/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:54:01 by abinet            #+#    #+#             */
/*   Updated: 2024/06/17 15:36:52 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::servCommand(int fd, std::string const &msg)
{
	t_cmd commands = ft_splitter(msg);
	Client *client = _client.at(fd);

	for (it_cmd it = commands.begin(); it != commands.end(); ++it)
	{
		std::string command_str = *it;
		std::istringstream iss(command_str);
		std::string command;
		iss >> command;

		std::string remaining;
		std::getline(iss, remaining);

		if (remaining[0] == ' ')
			remaining.erase(0, 1);

		if ((remaining.empty() || remaining == "\r\n"))
			servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(client->getNickname(), command));
		else if (command == "PING" || command == "/ping")
			servSend(_srv_sock, fd, RPL_PONG(client->getNickname()));
		else if (command == "PASS" || command == "/pass")
			pass(fd, remaining);
		else if ((command == "NICK" || command == "/nick"))
			nick(fd, remaining);
		else if ((command == "USER" || command == "/user"))
			user(fd, remaining);
		else if ((command == "PRIVMSG" || command == "/msg") && remaining.find("logbot") == 0)
			logBot(fd, remaining);
		else if (!client->getLogged())
			servSend(_srv_sock, fd, ERR_NOLOGIN(client->getHostname()));
		else if (command == "PRIVMSG" || command == "/msg")
			msgSend(fd, remaining);
		else if (command == "JOIN" || command == "/join")
			join(fd, remaining);
		else if (command == "KICK" || command == "/kick")
			kick(fd, remaining);
		else if (command == "MODE" || command == "/mode")
			mode(fd, remaining);
		else if (command == "TOPIC" || command == "/topic")
			topic(fd, remaining);
		else if (command == "INVITE" || command == "/invite")
			invite(fd, remaining);
		else if (command == "PART" || command == "/part")
			part(fd, remaining);
		else
			servSend(_srv_sock, fd, ERR_UNKNOWNCOMMAND(client->getNickname(), command));
	}
}
