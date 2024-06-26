/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:28:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/15 12:18:26 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

class Channel;

class Server
{
	protected:
		int			_srv_sock;
		int			_srv_port;
		std::string	_srv_password;

		t_poll		_poll;
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
		void		servCommand(int fd, std::string const &msg);
		void		servSend(int fd_src, int fd_dest, std::string const &msg);
		void		servClose(int fd);

		void		servSignal(void);

		void		setPort(int port);
		void		setPort(std::string const &input);
		void		setPassword(std::string const &input);

		int			getSock(void) const;
		int			getPort(void) const;
		std::string	getPassword(void) const;
		Channel		&getChannel(std::string const &name);

		bool		isClient(std::string const &name);
		Client		&getClient(std::string const &name);

		void		logBot(int fd, std::string const &msg);
		void		invite(int fd, std::string const &msg);
		void		join(int fd, std::string const &msg);
		void		kick(int fd, std::string const &msg);
		void		mode(int fd, std::string const &msg);
		void		modeMulti(int fd, std::istringstream &iss, Channel &channel, std::string &modes);
		void		modeK(int fd, std::istringstream &iss, Channel &channel, char action);
		void		modeO(int fd, std::istringstream &iss, Channel &channel, char action);
		void		modeL(int fd, std::istringstream &iss, Channel &channel, char action);
		void		msgSend(int fd, std::string const &msg);
		void		msgPrv(int fd, std::string const &name, std::string const &msg);
		void		msgChannel(int fd, std::string &channel_name, std::string const &msg);
		void		nick(int fd, std::string const &msg);
		void		part(int fd, std::string const &msg);
		void		pass(int fd, std::string const &msg);
		void		topic(int fd, std::string const &msg);
		void		user(int fd, std::string const &msg);
};

#endif
