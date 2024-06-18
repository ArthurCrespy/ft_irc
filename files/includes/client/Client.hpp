/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:17:09 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/10 14:06:48 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client
{
	private:
		int			_cli_fd;
		int			_cli_port;

		bool		_cli_p;
		bool		_cli_n;
		bool		_cli_u;
		bool		_cli_l;

		std::string	_cli_nickname;
		std::string	_cli_username;
		std::string	_cli_realname;
		std::string	_cli_hostname;

	public:
		Client(void);
		Client(int cli_fd, int cli_port, std::string const &hostname);
		Client(Client const &src);
		~Client(void);

		Client &operator=(Client const &rhs);

		void		setFd(int fd);
		void		setPort(int port);
		void		setPassword(bool pwd);
		void		setNick(bool nick);
		void		setUser(bool user);
		void		setLogged(bool logged);
		void		setNickname(std::string const &nickname);
		void		setUsername(std::string const &username);
		void		setRealname(std::string const &realname);
		void		setHostname(std::string const &hostname);

		int			getFd(void) const;
		int			getPort(void) const;
		bool		getPassword(void) const;
		bool		getNick(void) const;
		bool		getUser(void) const;
		bool		getLogged(void) const;
		std::string	getNickname(void) const;
		std::string	getUsername(void) const;
		std::string	getRealname(void) const;
		std::string	getHostname(void) const;
		std::string getPrefix(void) const;
};

#endif
