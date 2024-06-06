/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:28:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/05 20:01:16 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.h"

#ifndef SERVER_HPP
# define SERVER_HPP

class Server
{
	private:
		int			_srv_sock;
		int			_srv_port;
		std::string	_srv_password;

		t_poll		_poll;
		t_user		_user;
		t_client	_client;
		t_channel	_channel;
		t_signal	_signal;

	public:
		Server(void);
		Server(int argc, char **argv);
		Server(int port, std::string const &name);
		Server(Server const &src);
		~Server(void);

		Server &operator=(Server const &rhs);

		void		servStart(void);

		void		servSetup(int argc, char **argv);

		void		servListen(void);
		void		servPoll(void);
		void		servConnect(void);
		void		servReceive(int fd);
		void		servClose(int fd);

		void		servSignal(void);

		void		setPort(int port);
		void		setPort(std::string const &input);
		void		setPassword(std::string const &input);

		int			getPort(void) const;
		std::string	getPassword(void) const;
		int			getSock(void) const;
		Channel&	getchannel(const std::string& name_channel);

		void handleCommand(const std::string & msg, int fd);
		void handleJoin(const std::string &msg, int fd);
		void handlePrivMsg(const std::string &msg, int fd);
		void msg_prv(int fd, const std::string& name, const std::string& message);
		void msg_channel(int fd, const std::string& channel, std::string& message);

		void logBot(int fd, std::string const &msg);

		void ft_send(int fd, std::string const &msg, int flags);

		void kick(int fd, std::string const &msg);
		void topic(int fd, std::string const &msg);
};

#endif
