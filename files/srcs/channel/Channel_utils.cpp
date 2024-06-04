/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:46:27 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/04 17:40:15 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"


void Channel::setTopic(std::string const &topic)
{
	if (topic.empty())
		_channel_topic = "No topic yet...";
	else
		_channel_topic = topic;
}

void Channel::setTopicRestriction(bool restrict)
{
	_channel_topic_restrict = restrict;
}

void Channel::setPassword(std::string const &password)
{
	if (password.empty())
	{
		_channel_password = "";
		_channel_password_restrict = false;
	}
	else
	{
		_channel_password = password;
		_channel_password_restrict = true;
	}
}

void Channel::setPasswordRestriction(bool restrict)
{
	_channel_password_restrict = restrict;
}

void Channel::setLimit(int limit)
{
	_channel_limit = limit;
}

void Channel::setInviteOnly(bool io)
{
	_channel_invite_only = io;
}

std::string Channel::getName(void) const
{
	return (_channel_name);
}

std::string Channel::getTopic(void) const
{
	return (_channel_topic);
}

bool Channel::getTopicRestriction(void) const
{
	return (_channel_topic_restrict);
}

std::string Channel::getPassword(void) const
{
	return (_channel_password);
}

bool Channel::getPasswordRestriction(void) const
{
	return (_channel_password_restrict);
}

int Channel::getLimit(void) const
{
	return (_channel_limit);
}

bool Channel::getInviteOnly(void) const
{
	return (_channel_invite_only);
}

t_members Channel::getMembers(void) const
{
	return (_channel_members);
}

t_members Channel::getAdmins(void) const
{
	return (_channel_admins);
}

void Channel::addMember(Client *member)
{
	_channel_members.insert(std::make_pair(member->getNickname(), member));
}

void Channel::removeMember(Client *member)
{
	it_members it = _channel_members.find(member->getNickname());
	if (it != _channel_members.end())
	{
		removeAdmin(member);
		if (_channel_admins.empty())
			addAdmin(_channel_members.begin()->second);
		_channel_members.erase(it);
	}

}

void Channel::addAdmin(Client *op)
{
	op->ft_send(op->getFd(), RPL_CHANNELMODEIS(op->getNickname(), _channel_name, "+o ", op->getNickname()), 0);
	_channel_admins.insert(std::make_pair(op->getNickname(), op));
}

void Channel::removeAdmin(Client *op)
{
	it_members it = _channel_members.find(op->getNickname());
	if (it != _channel_admins.end())
	{
		(*it).second->ft_send((*it).second->getFd(), RPL_CHANNELMODEIS((*it).second->getNickname(), _channel_name, "-o ", op->getNickname()), 0);
		_channel_admins.erase(it);
	}
}

void Channel::broadcast(std::string const &msg)
{
	for (it_members it = _channel_members.begin(); it != _channel_members.end(); it++)
		(*it).second->ft_send((*it).second->getFd(), msg, 0);
}
