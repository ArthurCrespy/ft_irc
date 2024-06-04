/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:46:27 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/04 17:05:35 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

Channel::Channel(void)
{
	_channel_name = "";
	setTopic("");
	setTopicRestriction(false);
	setPassword("");
	setLimit(-1);
	setInviteOnly(false);
	ft_print("Channel created: " + _channel_name, LOG);
}

Channel::Channel(std::string const &name, Client *op) : _channel_name(name)
{
	setTopic("");
	setTopicRestriction(false);
	setPassword("");
	setLimit(-1);
	setInviteOnly(false);

	_channel_admins.insert(std::make_pair(op->getUsername(), op));
	_channel_members.insert(std::make_pair(op->getUsername(), op));

	ft_print("Channel created: " + _channel_name, LOG);
}

Channel::Channel(Channel const &src)
{
	*this = src;
}

Channel::~Channel(void)
{
	_channel_members.clear();
	_channel_admins.clear();
}

Channel &Channel::operator=(Channel const &rhs)
{
	if (this != &rhs)
	{
		this->_channel_name = rhs._channel_name;
		this->_channel_topic = rhs._channel_topic;
		this->_channel_topic_restrict = rhs._channel_topic_restrict;
		this->_channel_password = rhs._channel_password;
		this->_channel_limit = rhs._channel_limit;
		this->_channel_invite_only = rhs._channel_invite_only;
		this->_channel_members = rhs._channel_members;
		this->_channel_admins = rhs._channel_admins;
	}
	return (*this);
}
