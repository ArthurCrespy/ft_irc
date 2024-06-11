/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:43:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 15:12:40 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
# define REPLIES_H

/* 001-099 | Client-Server Exchange */
# define RPL_WELCOME(nickname)									"001 " + nickname +						" :Welcome to the 42IRC network"
# define RPL_YOURHOST(nickname)									"002 " + nickname +						" :Your host is 42IRC, running version 1.0"

/* 200-399 | Command Replies */
# define RPL_UMODEIS(nickname, mode)                            "221 " + nickname + " " + mode
# define RPL_CHANNELMODEIS(nickname, channel, modes)	        "324 " + nickname + " " + channel +		" " + modes
# define RPL_NOTOPIC(nickname, channel)							"331 " + nickname + " " + channel +		" :No topic is set"
# define RPL_TOPIC(nickname, channel, topic)					"332 " + nickname + " " + channel +		" :" + topic
# define RPL_NAMREPLY(nickname, channel, users)					"353 " + nickname + " = " + channel +	" :" + users
# define RPL_ENDOFNAMES(nickname, channel)						"366 " + nickname + " " + channel +		" :End of /NAMES list."

/* 400-599 | Error Replies */
# define ERR_NOSUCHNICK(nickname, nc)							"401 " + nickname + " " + nc +			" :No such nick/channel"
# define ERR_NOSUCHCHANNEL(nickname, channel)					"403 " + nickname + " " + channel +		" :No such channel"
# define ERR_CANNOTSENDTOCHAN(nickname, channel)				"404 " + nickname + " " + channel +		" :Cannot send to channel"
# define ERR_TOOMANYCHANNELS(nickname, channel)					"405 " + nickname + " " + channel +		" :You have joined too many channels"
# define ERR_NOTEXTTOSEND(nickname)								"412 " + nickname +						" :No text to send"
# define ERR_UNKNOWNCOMMAND(nickname, command)					"421 " + nickname + " " + command +		" :Unknown command"
# define ERR_NICKNAMEINUSE(nickname)							"433 " + nickname + " " + nickname	+	" :Nickname is already in use"
# define ERR_NONICKNAMEGIVEN(nickname)							"431 " + nickname +						" :No nickname given"
# define ERR_USERNOTINCHANNEL(nickname, channel)				"441 " + nickname + " " + channel +		" :They aren't on that channel"
# define ERR_NOTONCHANNEL(nickname, channel)					"442 " + nickname + " " + channel +		" :You're not on that channel"
# define ERR_NOLOGIN(nickname)									"444 " + nickname +						" :User not logged in"
# define ERR_NOTREGISTERED(nickname)							"451 " + nickname +						" :You have not registered"
# define ERR_NEEDMOREPARAMS(nickname, command)					"461 " + nickname + " " + command +		" :Not enough parameters"
# define ERR_ALREADYREGISTERED(nickname)						"462 " + nickname +						" :You may not register"
# define ERR_PASSWDMISMATCH(nickname)							"464 " + nickname +						" :Password incorrect"
# define ERR_CHANNELISFULL(nickname, channel)					"471 " + nickname + " " + channel +		" :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(nickname, modeChar)                    "472 " + nickname + " " + modeChar + " :is unknown mode char to me"
# define ERR_BADCHANNELKEY(nickname, channel)					"475 " + nickname + " " + channel +		" :Cannot join channel (+k)"
# define ERR_BADCHANMASK(channel)								"476 " + channel +						" :Bad Channel Mask"
# define ERR_CHANOPRIVSNEEDED(nickname, channel)				"482 " + nickname + " " + channel +		" :You're not channel operator"

/* Custom Replies */
# define RPL_JOIN(channel)										"JOIN :" + channel
# define RPL_PART(channel)										"PART :" + channel
# define RPL_PRIVMSG(target, message)							"PRIVMSG " + target + " :" + message
# define RPL_NOTICE(target, message)							"NOTICE " + target + " :" + message
# define RPL_QUIT(message)										"QUIT :Quit: " + message
# define RPL_KICK(channel, target, reason)						"KICK " + channel + " " + target + " :" + reason
# define RPL_MODE(channel, modes)								"MODE " + channel + " :" + modes

/* LogBot Replies */
# define RPL_LBRGST(command)									"Cannot do " + command + "! Not authenticated...\nUse LOGBOT, our bot! /msg LOGBOT <password> <nickname> <username> <realname>"
# define RPL_LBTREGISTER(nickname)								"Hey " + nickname + " ! You are now registered"
# define RPL_LBWELCOME(nickname, username, hostname)			"001 " + nickname +	" :Welcome to the 42IRC network " + nickname + "!" + username + "@" + hostname

/* PING/PONG Replies */
# define RPL_PING(nickname)										"PING " + nickname
# define RPL_PONG(nickname)										"PONG " + nickname + " :" + nickname

#endif
