/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc_typedef.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:43:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 11:00:22 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

#ifndef FT_IRC_TYPEDEF_H
# define FT_IRC_TYPEDEF_H

class Client;
class Channel;

typedef			socklen_t	t_sock_len;
typedef struct	sockaddr_in	t_sock_addr_in;
typedef	struct	pollfd		t_poll_fd;
typedef			siginfo_t	t_siginfo;
typedef struct	sigaction	t_signal;

typedef std::vector<char>							t_mode;
typedef std::vector<char>::iterator					it_mode;

typedef std::vector<std::string>					t_cmd;
typedef std::vector<std::string>::iterator			it_cmd;

typedef std::vector<t_poll_fd>						t_poll;
typedef std::vector<t_poll_fd>::iterator			it_poll;

typedef std::map<std::string, Client *>				t_members;
typedef std::map<std::string, Client *>::iterator	it_members;

typedef std::map<std::string, Client>				t_user;
typedef std::map<std::string, Client>::iterator		it_user;

typedef std::map<int, Client *>						t_client;
typedef std::map<int, Client *>::iterator			it_client;

typedef std::map<std::string, Channel>				t_channel;
typedef std::map<std::string, Channel>::iterator	it_channel;

#endif
