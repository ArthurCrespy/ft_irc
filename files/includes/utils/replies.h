/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:43:41 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/17 14:41:47 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
# define REPLIES_H

/* 001-099 | Client-Server Exchange */
# define RPL_WELCOME(nickname, username, hostname)				"001 " + nickname +						" :Welcome to the 42IRC network " + nickname + "!" + username + "@" + hostname
# define RPL_YOURHOST(nickname)									"002 " + nickname +						" :Your host is 42IRC, running version 1.0"

/* 200-399 | Command Replies */
# define RPL_UMODEIS(nickname, mode)                            "221 " + nickname + " " + mode
# define RPL_CHANNELMODEIS(nickname, channel, modes)	        "324 " + nickname + " #" + channel +	" " + modes
# define RPL_NOTOPIC(nickname, channel)							"331 " + nickname + " #" + channel +	" :No topic is set"
# define RPL_TOPIC(nickname, channel, topic)					"332 " + nickname + " #" + channel +	" :" + topic
# define RPL_INVITING(nickname, channel, target)				"341 " + nickname + " #" + channel +	" " + target

/* 400-599 | Error Replies */
# define ERR_NOSUCHNICK(nickname, nc)							"401 " + nickname + " " + nc +			" :No such nick/channel"
# define ERR_NOSUCHCHANNEL(nickname, channel)					"403 " + nickname + " #" + channel +	" :No such channel"
# define ERR_NOTEXTTOSEND(nickname)								"412 " + nickname +						" :No text to send"
# define ERR_UNKNOWNCOMMAND(nickname, command)					"421 " + nickname + " " + command +		" :Unknown command"
# define ERR_NONICKNAMEGIVEN(nickname)							"431 " + nickname +						" :No nickname given"
# define ERR_NICKNAMEINUSE(nickname, nc)						"433 " + nickname + " " + nc +			" :Nickname is already in use"
# define ERR_USERNOTINCHANNEL(nickname, channel)				"441 " + nickname + " #" + channel +	" :They aren't on that channel"
# define ERR_NOTONCHANNEL(nickname, channel)					"442 " + nickname + " #" + channel +	" :You're not on that channel"
# define ERR_USERONCHANNEL(nickname, channel) 					"443 " + nickname + " #" + channel +	" :is already on channel"
# define ERR_NOLOGIN(nickname)									"444 " + nickname +						" :User not logged in"
# define ERR_NOTREGISTERED(nickname)							"451 " + nickname +						" :You have not registered"
# define ERR_NEEDMOREPARAMS(nickname, command)					"461 " + nickname + " " + command +		" :Not enough parameters"
# define ERR_ALREADYREGISTERED(nickname)						"462 " + nickname +						" :You may not reregister"
# define ERR_PASSWDMISMATCH(nickname)							"464 " + nickname +						" :Password incorrect"
# define ERR_CHANNELISFULL(nickname, channel)					"471 " + nickname + " #" + channel +	" :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(nickname, mode)						"472 " + nickname + " " + mode +		" :is unknown mode char to me"
# define ERR_INVITEONLYCHAN(nickname, channel)					"473 " + nickname + " #" + channel +	" :Cannot join channel (+i)"
# define ERR_BADCHANNELKEY(nickname, channel)					"475 " + nickname + " #" + channel +	" :Cannot join channel (+k)"
# define ERR_BADCHANMASK(channel)								"476 " + channel +						" :Bad Channel Mask"
# define ERR_CHANOPRIVSNEEDED(nickname, channel)				"482 " + nickname + " #" + channel +	" :You're not channel operator"

/* Custom Replies */
# define RPL_PRIVMSG(nickname, message)							"PRIVMSG " + nickname + " :" + message
# define RPL_CHANNEL(channel, message)							"PRIVMSG #" + channel + " :" + message
# define RPL_KICK(channel, target, reason)						"KICK #" + channel + " " + target + " :" + reason
# define RPL_JOIN(nickname, channel)							"JOIN #" + channel
# define RPL_NICK(nickname, nc)									"NICK " + nickname + " :" + nc
# define RPL_PART(channel)										"PART #" + channel

/* LogBot Replies */
# define RPL_LBRGST(command)									"Cannot do " + command + "! Not authenticated...\nUse LOGBOT, our bot! /msg LOGBOT <password> <nickname> <username> <realname>"
# define RPL_LBLOGGED(nickname)									"Hey " + nickname + " ! You are now logged"

/* PING/PONG Replies */
# define RPL_PING(nickname)										"PING " + nickname
# define RPL_PONG(nickname)										"PONG " + nickname + " :" + nickname

#endif
