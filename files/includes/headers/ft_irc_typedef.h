/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc_typedef.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:43:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/15 16:43:57 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

#ifndef FT_IRC_TYPEDEF_H
# define FT_IRC_TYPEDEF_H

class Client;

typedef			socklen_t	t_sock_len;
typedef struct	sockaddr_in	t_sock_addr_in;
typedef	struct	pollfd		t_poll_fd;

typedef std::vector<t_poll_fd>				t_poll;
typedef std::vector<t_poll_fd>::iterator	it_poll;

typedef std::map<int, Client>				t_client;
typedef std::map<int, Client>::iterator		it_client;

#endif
