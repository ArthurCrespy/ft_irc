/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:46:27 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/24 09:48:34 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

Channel::Channel(void) {}

Channel::Channel(std::string const &name, Client *op) : _channel_name(name)
{
	setTopic("");
	setTopicRestriction(false);
	setPassword("");
	setLimit(-1);
	setInviteOnly(false);

	_channel_admins.push_back(op);
}

Channel::Channel(Channel const &src)
{
	*this = src;
}

Channel::~Channel(void) {}

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
	_channel_members.push_back(member);
}

void Channel::removeMember(Client *member)
{
	it_members it = std::find(_channel_members.begin(), _channel_members.end(), member);
	if (it != _channel_members.end())
		_channel_members.erase(it);
}

void Channel::addAdmin(Client *op)
{
	_channel_admins.push_back(op);
}

void Channel::removeAdmin(Client *op)
{
	it_members it = std::find(_channel_admins.begin(), _channel_admins.end(), op);
	if (it != _channel_admins.end())
		_channel_admins.erase(it);
}
