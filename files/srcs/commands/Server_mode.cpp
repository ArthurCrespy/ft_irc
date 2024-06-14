/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_mode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 14:36:45 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/13 16:28:18 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Server::mode(int fd, std::string const &msg)
{
	char action;
	std::string modes;
	std::string param;
	std::string channel_name;
	std::istringstream iss(msg);

	iss >> channel_name >> modes;
	if (channel_name.empty() || modes.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "MODE")));
	if (channel_name[0] != '#' && channel_name[0] != '&')
		return (servSend(_srv_sock, fd, ERR_UNKNOWNCOMMAND(_client.at(fd)->getNickname(), "MODE on user")));
	else
		channel_name.erase(0, 1);

	if (_channel.count(channel_name) != 0)
	{
		Channel &channel = _channel.at(channel_name);
		action = modes[0];
		if (!channel.isAdmin(_client.at(fd)))
			return (servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(_client.at(fd)->getNickname(), channel_name)));
		if (action != '+' && action != '-')
			return (servSend(_srv_sock, fd, ERR_UNKNOWNMODE(_client.at(fd)->getNickname(), modes)));
		modeMulti(fd, iss, channel, modes);
	}
	else
		servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), channel_name));
}

void Server::modeMulti(int fd, std::istringstream &iss, Channel &channel, std::string &modes)
{
	char action;
	std::string param;

	action = modes[0];
	for (size_t i = 1; i < modes.length(); ++i)
	{
		char mode = modes[i];
		if (mode == 'i')
		{
			channel.setInviteOnly(action == '+');
			servSend(_srv_sock, fd, RPL_CHANNELMODEIS(_client.at(fd)->getNickname(), channel.getName(), action + "i"));
		}
		else if (mode == 't')
		{
			channel.setTopicRestriction(action == '+');
			servSend(_srv_sock, fd, RPL_CHANNELMODEIS(_client.at(fd)->getNickname(), channel.getName(), action + "t"));
		}
		else if (mode == 'k')
			modeK(fd, iss, channel, action);
		else if (mode == 'o')
			modeO(fd, iss, channel, action);
		else
			servSend(_srv_sock, fd, ERR_UNKNOWNMODE(_client.at(fd)->getNickname(), std::string(1, mode)));
	}
}

void Server::modeK(int fd, std::istringstream &iss, Channel &channel, char action)
{
	std::string param;

	if (action == '+')
	{
		if (!(iss >> param))
			return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "MODE")));
		channel.setPassword(param);
		channel.setPasswordRestriction(true);
	}
	else
		channel.setPasswordRestriction(false);
	servSend(_srv_sock, fd, RPL_CHANNELMODEIS(_client.at(fd)->getNickname(), channel.getName(), action + "k"));
}

void Server::modeO(int fd, std::istringstream &iss, Channel &channel, char action)
{
	std::string param;

	if (!(iss >> param))
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "MODE")));

	if (isClient(param))
	{
		Client &client = getClient(param);
		if (action == '+')
			channel.addAdmin(&client);
		else if (action == '-')
			channel.removeAdmin(&client);
		servSend(_srv_sock, fd, RPL_CHANNELMODEIS(_client.at(fd)->getNickname(), channel.getName(), action + "o"));
	}
}
