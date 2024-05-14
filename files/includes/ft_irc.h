/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 10:35:05 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/14 10:36:53 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define NONE "    "
# define INPUT "\033[1;35mINPT\033[0m"
# define INFO "\033[1;34mINFO\033[0m"
# define LOG "\033[1;36mLOGG\033[0m"
# define WARN "\033[1;33mWARN\033[0m"
# define ERROR "\033[1;31mERRR\033[0m"
# define RUN "\033[1;32mRUNN\033[0m"
# define STOP "\033[1;31mSTOP\033[0m"

# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/poll.h>
# include <stdexcept>
# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <ctime>

# include "./../includes/Server.hpp"

std::string	ft_nbtos(const int &value);
int 		ft_stoi(const std::string &str);
void		ft_print(const std::string &str, const std::string &level);


