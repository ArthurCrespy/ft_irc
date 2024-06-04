/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:17:09 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/04 13:43:13 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <deque>
#include <string>
#include"../channel/User.hpp"

class User;
class Server;

class Client
{
	protected:
		int				_cli_fd;
		int				_cli_port;

		std::string		_cli_nickname;
		std::string		_cli_username;
		std::string		_cli_realname;
		std::string		_cli_hostname;
		std::map<int, User>				_users;

	public:
		Client(void);
		Client(int cli_fd, int cli_port, std::string const &hostname);
		Client(Client const &src);
		~Client(void);

		Client &operator=(Client const &rhs);

		void	cliReceive(std::string const &msg, int fd, Server & server);

		void	setFd(int fd);
		void	setPort(int port);
		void	setNickname(std::string const &nickname);
		void	setUsername(std::string const &username);
		void	setRealname(std::string const &realname);
		void	setHostname(std::string const &hostname);

		int		getFd(void) const;
		int		getPort(void) const;
		std::string	getNickname(void) const;
		std::string	getUsername(void) const;
		std::string	getRealname(void) const;
		std::string	getHostname(void) const;
		std::string getPrefix(void) const;

		User							*findUserByFd(int fd);
		std::deque<std::string> 		split(std::string const &msg, std::string const &delimiters);

		void ft_send(int fd, std::string const &msg, int flags);

		void ExecJoinTEST(const std::string &msg, int fd, Server & server, User &user);
		void handlePrivMsg(const std::string &msg, int fd);
		void msg_prv(int fd, const std::string& name, const std::string& message);
		void msg_channel(int fd, const std::string& channel, const std::string& message);
};

#endif
