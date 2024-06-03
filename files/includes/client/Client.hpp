/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinet <abinet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:17:09 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/22 00:42:04 by abinet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client
{
	private:
		int				_cli_fd;
		int				_cli_port;

		bool			_cli_registered;

		std::string		_cli_nickname;
		std::string		_cli_username;
		std::string		_cli_realname;
		std::string		_cli_hostname;
		std::string		_cli_password;

	public:
		Client(void);
		Client(int cli_fd, int cli_port, std::string const &hostname);
		Client(Client const &src);
		~Client(void);

		Client &operator=(Client const &rhs);

		void cliReceive(int fd, std::string const &msg);

		void	setFd(int fd);
		void	setPort(int port);
		void    setRegistration(bool reg);
		void	setNickname(std::string const &nickname);
		void	setUsername(std::string const &username);
		void	setRealname(std::string const &realname);
		void	setHostname(std::string const &hostname);

		int		getFd(void) const;
		int		getPort(void) const;
		bool	getRegistration(void) const;
		std::string	getNickname(void) const;
		std::string	getUsername(void) const;
		std::string	getRealname(void) const;
		std::string	getHostname(void) const;
		std::string getPrefix(void) const;


		void ft_send(int fd, std::string const &msg, int flags);

		void handlePrivMsg(const std::string &msg, int fd);
		void msg_prv(int fd, const std::string& name, const std::string& message);
		void msg_channel(int fd, const std::string& channel, const std::string& message);

		void    logBot(int fd, std::string const &msg);
};

#endif
