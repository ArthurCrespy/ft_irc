/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:57:30 by jdegluai          #+#    #+#             */
/*   Updated: 2024/06/04 13:37:31 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/channel/Join.hpp"
#include "../../includes/client/Client.hpp"

JoinCommand::JoinCommand(void)  /*  : CallCom("JOIN")*/ {}

JoinCommand::~JoinCommand(void) {}

std::string	JoinCommand::getStringifiedUsers(Channel *channel) const
{
    (void)channel;
    return "";
}

std::string	JoinCommand::getChannelMode(Channel *channel) const
{
    (void)channel;
    return "";
}

void	JoinCommand::leaveAllChannels(void) const {
}

// void    JoinCommand::ExecJoin(User *user, std::deque<std::string> commandArgs)
// {
//     (void)user;
//     (void)commandArgs;
// }

// void JoinCommand::handleJoin(const std::string &msg, int fd, Server & server)
// {
// 	size_t start = msg.find("PRIVMSG") + 8;
// 	// size_t end = msg.find(" ", start);
// 	(void)fd;
// 	(void)server;
// 	std::string channel = msg;
// 	// if (channel[0] != '#' || channel[0] != '&')
// 	// 	return ft_send(fd, ERR_NOSUCHCHANNEL(getNickname(), channel), 0);
// 	channel.erase(0, 1);
// }
