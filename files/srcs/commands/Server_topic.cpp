/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_topic.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:53:00 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/06 14:53:00 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

/**
	*  @brief	Change the topic of a channel
	*  @param	int fd : file descriptor of the client socket
	*  @param	std::string msg : message received from the client
	*  @return	None
*/
void Server::topic(int fd, std::string const &msg)
{
	std::string channel_name;
	std::string topic;
	std::istringstream iss(msg);

	iss >> channel_name >> topic;
	if (channel_name.empty())
		servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(_client.find(fd)->second->getNickname(), "TOPIC"));
	else if (topic.empty())
	{
		if (_channel.find(channel_name) == _channel.end())
			servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.find(fd)->second->getNickname(), channel_name));
		else if (_channel.find(channel_name)->second.getMembers().find(_client.find(fd)->second->getNickname()) == _channel.find(channel_name)->second.getMembers().end())
			servSend(_srv_sock, fd, ERR_NOTONCHANNEL(_client.find(fd)->second->getNickname(), channel_name));
		else if (_channel.find(channel_name)->second.getTopic().empty())
			servSend(_srv_sock, fd, RPL_NOTOPIC(_client.find(fd)->second->getNickname(), channel_name));
		else
			servSend(_srv_sock, fd, RPL_TOPIC(_client.find(fd)->second->getNickname(), channel_name,
			                          _channel.find(channel_name)->second.getTopic()));
	}
	else
	{
		if (_channel.find(channel_name) == _channel.end())
			servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(_client.find(fd)->second->getNickname(), channel_name));
		else if (_channel.find(channel_name)->second.getMembers().find(_client.find(fd)->second->getNickname()) == _channel.find(channel_name)->second.getMembers().end())
			servSend(_srv_sock, fd, ERR_NOTONCHANNEL(_client.find(fd)->second->getNickname(), channel_name));
		else if (_channel.find(channel_name)->second.getAdmins().find(_client.find(fd)->second->getNickname()) == _channel.find(channel_name)->second.getAdmins().end())
			servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(_client.find(fd)->second->getNickname(), channel_name));
		else
		{
			_channel.find(channel_name)->second.setTopic(topic);
			servSend(_srv_sock, fd, RPL_TOPIC(_client.find(fd)->second->getNickname(), channel_name, topic));
		}
	}
}
