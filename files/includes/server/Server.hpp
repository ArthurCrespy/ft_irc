/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:28:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/04 11:15:32 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<map>
#include "../ft_irc.h"
#include "../channel/User.hpp"

#ifndef SERVER_HPP
# define SERVER_HPP

class User;

class Server
{
	private:
		int			_srv_sock;
		int			_srv_port;
		std::string	_srv_password;

		t_poll		_poll;
		t_client	_client;
		t_channel	_channel;
		t_signal 	_signal;

	public:
		Server(void);
		Server(int argc, char **argv);
		Server(int port, std::string const &name);
		Server(Server const &src);
		~Server(void);

		// User							*findUserByFd(int fd);
		Server &operator=(Server const &rhs);

		void		servStart(void);

		void		servSetup(int argc, char **argv);

		void		servListen(void);
		void		servPoll(void);
		void		servConnect(void);
		void		servReceive(std::vector<pollfd>::iterator &it);
		void		servClose(int fd);

		void		servSignal(void);

		void		setPort(int port);
		void		setPort(std::string const &input);
		void		setPassword(std::string const &input);

		int			getPort(void) const;
		std::string	getPassword(void) const;
		int			getSock(void) const;
		// User*			findUserByFd(int fd);
};

#endif
