/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:39:06 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/15 16:39:06 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

#ifndef UTILS_H
# define UTILS_H

std::string	ft_nbtos(int const &value);
int 		ft_stoi(std::string const &str);
const char *ft_stov(std::string const &str);


void		ft_print(std::string const &str,std::string const &level);

void		ft_servSHandler(int sig, t_siginfo *siginfo, void *content);

size_t		ft_strlen(std::string const &str);

#endif
