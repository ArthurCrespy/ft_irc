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

void Server::topic(int fd, std::string const &msg)
{
	std::string channel_name;
	std::string topic;
	std::istringstream iss(msg);

	iss >> channel_name >> topic;
	if (channel_name.empty())
		ft_send(fd, ERR_NEEDMOREPARAMS(_client.find(fd)->second->getNickname(), "TOPIC"), 0);
	else if (topic.empty())
	{
		if (_channel.find(channel_name) == _channel.end())
			ft_send(fd, ERR_NOSUCHCHANNEL(_client.find(fd)->second->getNickname(), channel_name), 0);
		else if (_channel.find(channel_name)->second.getMembers().find(_client.find(fd)->second->getNickname()) == _channel.find(channel_name)->second.getMembers().end())
			ft_send(fd, ERR_NOTONCHANNEL(_client.find(fd)->second->getNickname(), channel_name), 0);
		else if (_channel.find(channel_name)->second.getTopic().empty())
			ft_send(fd, RPL_NOTOPIC(_client.find(fd)->second->getNickname(), channel_name), 0);
		else
			ft_send(fd, RPL_TOPIC(_client.find(fd)->second->getNickname(), channel_name, _channel.find(channel_name)->second.getTopic()), 0);
	}
	else
	{
		if (_channel.find(channel_name) == _channel.end())
			ft_send(fd, ERR_NOSUCHCHANNEL(_client.find(fd)->second->getNickname(), channel_name), 0);
		else if (_channel.find(channel_name)->second.getMembers().find(_client.find(fd)->second->getNickname()) == _channel.find(channel_name)->second.getMembers().end())
			ft_send(fd, ERR_NOTONCHANNEL(_client.find(fd)->second->getNickname(), channel_name), 0);
		else if (_channel.find(channel_name)->second.getAdmins().find(_client.find(fd)->second->getNickname()) == _channel.find(channel_name)->second.getAdmins().end())
			ft_send(fd, ERR_CHANOPRIVSNEEDED(_client.find(fd)->second->getNickname(), channel_name), 0);
		else
		{
			_channel.find(channel_name)->second.setTopic(topic);
			ft_send(fd, RPL_TOPIC(_client.find(fd)->second->getNickname(), channel_name, topic), 0);
		}
	}
}
