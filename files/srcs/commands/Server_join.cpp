/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:52:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/09 12:52:43 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

// --- merged from abinet --- //
void Server::join(int fd, std::string const &msg)
{
	std::istringstream iss(msg);

	std::string name_channel;
	std::string message;

	iss >> name_channel;

	if (name_channel.empty())
		return servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN"));
	if (name_channel[0] != '#' && name_channel[0] !='&')
		return servSend(_srv_sock, fd, ERR_BADCHANMASK(name_channel));
	name_channel.erase(0, 1);

	it_channel it;
	it = _channel.find(name_channel);
	if (it != _channel.end())
	{
		if (_channel.at(name_channel).getPasswordRestriction())
		{
			std::string mdp;
			iss >> mdp;
			if (mdp.empty() || _channel.at(name_channel).getPassword() != mdp)
				return servSend(_srv_sock, fd, ERR_PASSWDMISMATCH(_client.at(fd)->getNickname()));
		}
		_channel.at(name_channel).addMember(_client.at(fd));
		servSend(_srv_sock, fd, "You have joined the channel: " + name_channel);
		_channel.at(name_channel).broadcast(_client.at(fd)->getNickname() + " has joined the channel.");
	}
	else
	{
		Channel newChannel(name_channel, _client.at(fd));
		_channel.insert(std::make_pair(name_channel, newChannel));
		_channel.at(name_channel).addMember(_client.at(fd));
		servSend(_srv_sock, fd, "You have joined the channel: " + name_channel);
	}
	// std::getline(iss, message); // normalement ca ne sert a rien si il y a d'autres messages apres
}

/* --- merged from acrespy --- //
void Server::join(int fd, std::string const &msg)
{
	std::string channel = msg;
	if (channel[0] != '#' || channel[0] != '&')
		return (servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.at(fd)->getNickname(), channel)));
	channel.erase(0, 1);
}

// --- merged from jdegluai --- //
std::deque<std::string>	Server::split(std::string message, std::string delimiters)
{
	std::deque<std::string>		args;
	std::string					token;
	size_t						pos = 0;

	while ((pos = message.find_first_of(delimiters)) != std::string::npos) {
		token = message.substr(0, pos);

		if (!token.empty())
			args.push_back(token);
		message.erase(0, pos + 1);
	}
	if (!message.empty())
		args.push_back(message);
	return (args);
}
void Server::join(int fd, std::string const &msg)
{
	std::deque<std::string> channels = this->split(msg, " ");
	std::string password;
	if (channels.size() < 1)
		return servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN"));
	// std::cout << "Channels received:" << std::endl;
	// for (std::deque<std::string>::const_iterator it = channel.begin(); it != channel.end(); ++it)
	//     std::cout << *it << std::endl;
	if (channels.size() > 2)
		password = channels[2];
	else
		password = std::string();

	// if (channels.empty())
	//     return ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN"), 0);

	std::string channelName = channels.front();
	if (channelName.at(0) != '#') {
		return servSend(_srv_sock, fd, ERR_BADCHANMASK(channelName));
	}

	if (_channel.find(channelName) != _channel.end()) {

		Channel &existingChannel = _channel[channelName];
		if (!existingChannel.getPassword().empty() && existingChannel.getPassword() != password)
			return servSend(_srv_sock, fd, ERR_BADCHANNELKEY(_client.at(fd)->getNickname(), channelName));

		existingChannel.addMember(_client.at(fd));
		servSend(_srv_sock, fd, "You have joined the channel: " + channelName);
		// existingChannel.broadcast(_client.at(fd)->getNickname() + " has joined the channel.");
	}
	else {
		Channel newChannel(channelName, _client.at(fd));
		// newChannel.
		newChannel.setOwner(_client.at(fd));
		newChannel.addAdmin(_client.at(fd));
		if (!password.empty())
			newChannel.setPassword(password);
		_channel[channelName] = newChannel;
		newChannel.addMember(_client.at(fd));
	}
	// channel.erase(0, 1);

	(void)fd;
}*/
