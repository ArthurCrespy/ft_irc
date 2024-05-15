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

# define NONE	"    "
# define INPUT	"\033[1;35mINPT\033[0m"
# define INFO	"\033[1;34mINFO\033[0m"
# define LOG	"\033[1;36mLOGG\033[0m"
# define RUN	"\033[1;32mRUNN\033[0m"
# define WARN	"\033[1;33mWARN\033[0m"
# define ERROR	"\033[1;31mERRR\033[0m"
# define STOP	"\033[1;31mSTOP\033[0m"

# define SRV_MAX 42

# include <sys/socket.h>	// socket, bind, listen, accept, connect
# include <arpa/inet.h>		// htons, htonl, ntohs, ntohl
# include <sys/poll.h>		// poll
# include <unistd.h>		// close
# include <netdb.h>			// gethostbyname
# include <fcntl.h>			// fcntl

# include <iostream>		// std::cout, std::endl
# include <sstream>			// std::stringstream

# include <stdexcept>		// std::runtime_error
# include <cstring>			// std::strerror
# include <cerrno>			// errno

# include <vector>			// std::vector

# include <ctime>			// std::time_t


# include "./../includes/Server.hpp"

std::string	ft_nbtos(const int &value);
int 		ft_stoi(const std::string &str);
void		ft_print(const std::string &str, const std::string &level);


