/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 12:52:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 11:58:06 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

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
		_channel.at(name_channel).broadcast(_client.at(fd)->getNickname(), _client.at(fd)->getNickname() + " has joined the channel.");
	}
	else
	{
		Channel newChannel(name_channel, _client.at(fd));
		_channel.insert(std::make_pair(name_channel, newChannel));
		servSend(_srv_sock, fd, "You have joined the channel: " + name_channel);
	}
	// std::getline(iss, message); // normalement ca ne sert a rien si il y a d'autres messages apres
}

// merged from jdegluai
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
void Server::join0(int fd, std::string const &msg)
{
	std::deque<std::string> channels = this->split(msg, " ");
	std::string password;
	if (channels.size() < 1)
		return servSend(fd, -1, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN"));
	// std::cout << "Channels received:" << std::endl;
	// for (std::deque<std::string>::const_iterator it = channel.begin(); it != channel.end(); ++it)
	//     std::cout << *it << std::endl;
	if (channels.size() > 1)
		password = channels[1];
	else
		password = std::string();

	std::cout << password << std::endl;

	// if (channels.empty())
	//     return ft_send(fd, ERR_NEEDMOREPARAMS(_client.at(fd)->getNickname(), "JOIN"), 0);

	std::string channelName = channels.front();
	if (channelName.at(0) != '#') {
		return servSend(fd, -1, ERR_BADCHANMASK(channelName));
	}

	if (_channel.find(channelName) != _channel.end()) {

		
		Channel &existingChannel = _channel[channelName];
		
		if (existingChannel.hasMode('k') && existingChannel.getPassword() != password) {
				return servSend(fd, -1, ERR_BADCHANNELKEY(_client.at(fd)->getNickname(), channelName));
			}
		if (existingChannel.hasMode('l')) {
			if (existingChannel.getMembers().size() >= static_cast<size_t>(existingChannel.getLimit()))
				return servSend(fd, -1, ERR_CHANNELISFULL(_client.at(fd)->getNickname(), channelName));
		}
		if (existingChannel.hasMode('i') && !existingChannel.isInvited(this->The_client)) {
			return servSend(fd, -1, ERR_BADCHANNELKEY(_client.at(fd)->getNickname(), channelName));
		}
			
		// if (!existingChannel.getPassword().empty() && existingChannel.getPassword() != password)
		// 	return servSend(fd, -1, ERR_BADCHANNELKEY(_client.at(fd)->getNickname(), channelName));
		existingChannel.addMember(_client.at(fd));
		servSend(fd, -1, "You have joined the channel: " + channelName);
	}
	else {
		Channel newChannel(channelName, _client.at(fd));
		newChannel.setOwner(_client.at(fd));
		newChannel.addAdmin(_client.at(fd));
		if (!password.empty())
			newChannel.setPassword(password);
		_channel[channelName] = newChannel;
		newChannel.addMember(_client.at(fd));
	}
	// channel.erase(0, 1);

	(void)fd;
}

