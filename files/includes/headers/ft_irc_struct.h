/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 11:09:54 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/16 11:09:54 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

#ifndef FT_IRC_STRUCT_H
# define FT_IRC_STRUCT_H

struct compare_fd
{
    bool operator() (pollfd const lhs, pollfd const &rhs) const
    {
        return (lhs.fd < rhs.fd);
    }
};

#endif
