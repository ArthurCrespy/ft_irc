/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc_include.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:41:48 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/15 16:41:48 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_INCLUDE_H
# define FT_IRC_INCLUDE_H

# include <sys/socket.h>	// socket, bind, listen, accept, connect
# include <arpa/inet.h>		// htons, htonl, ntohs, ntohl
# include <sys/poll.h>		// poll
# include <unistd.h>		// close
# include <netdb.h>			// gethostbyname
# include <fcntl.h>			// fcntl

# include <csignal>	    	// sigaction

# include <iostream>		// std::cout, std::endl
# include <sstream>			// std::stringstream

# include <stdexcept>		// std::runtime_error
# include <cstring>			// std::strerror
# include <cerrno>			// errno

# include <vector>			// std::vector
# include <map>				// std::map

# include <ctime>			// std::time_t

#endif
