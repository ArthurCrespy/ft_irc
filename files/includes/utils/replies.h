/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:43:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/28 09:58:59 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
# define REPLIES_H

/* Error Responses */
# define ERR_NONICKNAMEGIVEN(nickname)							"431 " + nickname + " :No nickname given"
# define ERR_NICKNAMEINUSE(nickname)							"433 " + nickname + " " + nickname	+ " :Nickname is already in use"
# define ERR_NOLOGIN(nickname)									"444 " + nickname + " :User not logged in"
# define ERR_NOTREGISTERED(nickname)							"451 " + nickname + " :You have not registered"
# define ERR_ALREADYREGISTERED(nickname)						"462 " + nickname + " :You may not register"
# define ERR_PASSWDMISMATCH(nickname)							"464 " + nickname + " :Password incorrect"

# define ERR_UNKNOWNCOMMAND(nickname, command)					"421 " + nickname + " " + command + " :Unknown command"
# define ERR_NEEDMOREPARAMS(nickname, command)					"461 " + nickname + " " + command + " :Not enough parameters"

# define ERR_NOSUCHCHANNEL(nickname, channel)					"403 " + nickname + " " + channel + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(nickname, channel)				"404 " + nickname + " " + channel + " :Cannot send to channel"
# define ERR_TOOMANYCHANNELS(nickname, channel)					"405 " + nickname + " " + channel + " :You have joined too many channels"
# define ERR_NOTONCHANNEL(nickname, channel)					"442 " + nickname + " " + channel + " :You're not on that channel"
# define ERR_CHANNELISFULL(nickname, channel)					"471 " + nickname + " " + channel + " :Cannot join channel (+l)"
# define ERR_BADCHANNELKEY(nickname, channel)					"475 " + nickname + " " + channel + " :Cannot join channel (+k)"
# define ERR_CHANOPRIVSNEEDED(nickname, channel)				"482 " + nickname + " " + channel + " :You're not channel operator"

# define ERR_NOSUCHNICK(nickname, nc)							"401 " + nickname + " " + nc + " :No such nick/channel"
# define ERR_USERNOTINCHANNEL(nickname, channel)				"441 " + nickname + " " + channel + " :They aren't on that channel"

# define ERR_NOTEXTTOSEND(nickname)								"412 " + nickname + " :No text to send"


/* Numeric Responses */
# define RPL_CHANNELMODEIS(nickname, channel, modes, params)	"324 " + nickname + channel + " " + modes + " " + params
# define RPL_NAMREPLY(nickname, channel, users) /*???*/			"353 " + nickname + " = " + channel + " :" + users
# define RPL_ENDOFNAMES(nickname, channel)						"366 " + nickname + " " + channel + " :End of /NAMES list."

/* Command Responses */
# define RPL_JOIN(nickname, channel)							":" + nickname + " JOIN :" + channel
# define RPL_PART(nickname, channel)							":" + nickname + " PART :" + channel
# define RPL_PING(nickname, token)								":" + nickname + " PONG :" + token
# define RPL_PRIVMSG(nickname, target, message)					":" + nickname + " PRIVMSG " + target + " :" + message
# define RPL_NOTICE(nickname, target, message)					":" + nickname + " NOTICE " + target + " :" + message
# define RPL_QUIT(nickname, message)							":" + nickname + " QUIT :Quit: " + message
# define RPL_KICK(nickname, channel, target, reason)			":" + nickname + " KICK " + channel + " " + target + " :" + reason
# define RPL_MODE(nickname, channel, modes, args)				":" + nickname + " MODE " + channel + " " + modes + " " + args

/* Custom Responses */
# define RPL_WELCOME(nickname)									"001 " + nickname + " :Welcome to the 42IRC network"
# define RPL_LBRGST(command)									"Cannot do " + command + "! Not authenticated...\nUse LOGBOT, our bot! /msg LOGBOT <password> <nickname> <username> <realname>"
# define RPL_LBTREGISTER(nickname)								"Welcome " + nickname + " ! You are now registered"

# define RPL_PONG(nickname) /*test*/							"PONG " + nickname + " :" + nickname

#endif
