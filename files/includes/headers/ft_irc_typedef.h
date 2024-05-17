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

typedef	socklen_t	t_socklen;

typedef	struct pollfd		t_pollfd;
typedef struct sockaddr_in	t_sockaddr_in;

typedef std::map<pollfd *, Client *>::iterator		it_pclimap;
typedef std::map<t_pollfd *, Client *, compare_fd>	t_pclimap;

#endif
