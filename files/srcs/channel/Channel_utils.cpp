/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:46:27 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/18 10:33:54 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

void Channel::setTopic(std::string const &topic)
{
	if (topic.empty())
	{
		_channel_topic = "No topic is set";
		_channel_topic_set = false;
	}
	else
	{
		_channel_topic = topic;
		_channel_topic_set = true;
	}
}

void Channel::setTopicSet(bool set)
{
	_channel_topic_set = set;
}

void Channel::setTopicRestriction(bool restrict)
{
	_channel_topic_restrict = restrict;
}

void Channel::setPassword(std::string const &password)
{
	if (password.empty())
		_channel_password_restrict = false;
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

bool Channel::getTopicSet(void) const
{
	return (_channel_topic_set);
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

bool Channel::isInvited(Client *member)
{
	for (it_members it = _channel_invite.begin(); it != _channel_invite.end(); ++it)
	{
		if (it->second == member)
			return (true);
	}
	return (false);
}

void Channel::addInvite(Client *member)
{
	_channel_invite.insert(std::make_pair(member->getNickname(), member));
}

void Channel::removeInvite(Client *member)
{
	it_members it = _channel_invite.find(member->getNickname());
	if (it != _channel_invite.end())
		_channel_invite.erase(it);
}

bool Channel::isMember(Client *member)
{
    for (it_members it = _channel_members.begin(); it != _channel_members.end(); ++it)
	{
        if (it->second == member)
            return (true);
    }
    return (false);
}

void Channel::addMember(Client *member)
{
	_channel_members.insert(std::make_pair(member->getNickname(), member));
}

void Channel::removeMember(Client *member)
{
	it_members it = _channel_members.find(member->getNickname());
	if (it != _channel_members.end())
		_channel_members.erase(it);
}

void Channel::removeMember(std::string const &member)
{
	it_members it = _channel_members.find(member);
	if (it != _channel_members.end())
		_channel_members.erase(it);
}

bool Channel::isAdmin(Client *member)
{
    for (it_members it = _channel_admins.begin(); it != _channel_admins.end(); ++it)
	{
        if (it->second == member)
            return (true);
    }
    return (false);
}

void Channel::addAdmin(Client *op)
{
	it_members it_members = _channel_members.find(op->getNickname());
	if (it_members != _channel_members.end())
		_channel_admins.insert(std::make_pair(op->getNickname(), op));
}

void Channel::removeAdmin(Client *op)
{
	it_members it_admin = _channel_admins.find(op->getNickname());
	if (it_admin != _channel_admins.end())
		_channel_admins.erase(it_admin);
}

void Channel::removeAdmin(const std::string &op)
{
	it_members it_admin = _channel_admins.find(op);
	if (it_admin != _channel_admins.end())
		_channel_admins.erase(it_admin);
}

void Channel::broadcast(std::string const &name, std::string const &msg)
{
	for (it_members it = _channel_members.begin(); it != _channel_members.end(); it++)
	{
		if (!(*it).second)
			continue ;
		if ((*it).second->getNickname() == name)
			continue ;
		chaSend(name, (*it).second->getFd(), msg);
	}
}

void Channel::chaSend(std::string const &name_src, int fd_dest, std::string const &msg)
{
	std::string str;

	str = ":" + _channel_members.at(name_src)->getPrefix() + " " + msg + "\r\n";

	if (send(fd_dest, str.c_str(), ft_strlen(str), 0) == -1)
		throw std::runtime_error("Syscall send() Failed in send: " + std::string(std::strerror(errno)));
}

