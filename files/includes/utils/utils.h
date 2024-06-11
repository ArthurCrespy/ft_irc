/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:39:06 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 14:02:04 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

#ifndef UTILS_H
# define UTILS_H

std::string	ft_nbtos(int const &value);
int 		ft_stoi(std::string const &str);

void		ft_print(std::string const &str,std::string const &level);

void		ft_servSHandler(int sig, t_siginfo *siginfo, void *content);

size_t		ft_strlen(std::string const &str);

#endif
