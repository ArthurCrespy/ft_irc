/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:28:30 by jdegluai          #+#    #+#             */
/*   Updated: 2024/06/04 13:36:39 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include <iostream>
# include <sstream>
# include <iomanip>
# include <ctime>
# include <cstring>
# include <algorithm>
# include <map>
# include <vector>
# include <deque>
# include <list>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <poll.h>
# include <unistd.h>
# include <signal.h>
# include <stddef.h>
# include "Channel.hpp"
#include "../client/Client.hpp"


class JoinCommand {

	private:
		void		leaveAllChannels(void) const;

	public:
		JoinCommand(void);
		~JoinCommand(void);

		std::string	getStringifiedUsers(Channel *channel) const;
		std::string	getChannelMode(Channel *channel) const;
		void ExecJoinTEST(const std::string &msg, int fd, Server & server, User &user);
		// void handleJoin(const std::string &msg, int fd, Server & server);

};

#endif
