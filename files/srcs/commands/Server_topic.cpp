/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_topic.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:53:00 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/15 11:59:28 by abinet           ###   ########.fr       */
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
	std::string topic;
	std::string nickname;
	std::string channel_name;
	std::istringstream iss(msg);

	iss >> channel_name;
	std::getline(iss, topic);

	Client *client = _client.at(fd);
	nickname = client->getNickname();
	if (channel_name[0] == '#' || channel_name[0] == '&')
		channel_name.erase(0, 1);
	if (channel_name.empty())
		return (servSend(_srv_sock, fd, ERR_NEEDMOREPARAMS(nickname, "TOPIC")));
	if (topic[0] == ':')
		topic.erase(0, 1);

	if (_channel.count(channel_name) != 0)
	{
		Channel &channel = _channel.at(channel_name);
		if (channel.isMember(client))
		{
			if (topic.empty())
			{
				if (channel.getTopic().empty())
					servSend(_srv_sock, fd, RPL_NOTOPIC(nickname, channel_name));
				else
					servSend(_srv_sock, fd, RPL_TOPIC(nickname, channel_name, channel.getTopic()));
			}
			else
			{
				if (channel.getTopicRestriction() && !channel.isAdmin(client))
					servSend(_srv_sock, fd, ERR_CHANOPRIVSNEEDED(nickname, channel_name));
				else
				{
					channel.setTopic(topic);
					servSend(_srv_sock, fd, RPL_TOPIC(nickname, channel_name, topic));
				}
			}
		}
		else
			servSend(_srv_sock, fd, ERR_NOTONCHANNEL(nickname, channel_name));
	}
	else
		servSend(_srv_sock, fd, ERR_NOSUCHCHANNEL(nickname, channel_name));
}
