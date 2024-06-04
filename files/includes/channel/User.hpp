/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:37:44 by jdegluai          #+#    #+#             */
/*   Updated: 2024/06/04 13:50:51 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include"../server/Server.hpp"

class Client;
class Server;

class User {

	private:
		/* Attributes */
		std::string	_host;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_readBuffer;
		std::string	_sendBuffer;
		int			_port;
		int			_socket;
		bool		_registered;
		bool		_sentPassword;

		/* Private constructor */
		User(void);

	public:
		/* Constructors & Destructors */
		User(std::string &host, int port, int socket);
		User(User const &origin);
		~User(void);

		/* Functions */
		// void	tryRegister(Server *server);
		// void	sendDirectMessage(std::string message);

		/* Getters & Setters */
		std::string	getHost(void) const;
		std::string	getNickname(void) const;
		std::string	getUsername(void) const;
		std::string	getRealname(void) const;
		std::string	getReadBuffer(void) const;
		std::string	getSendBuffer(void) const;
		int			getPort(void) const;
		int			getSocket(void) const;
		bool		isRegistered(void) const;
		bool		hasSentPassword(void) const;
		
		void		setNickname(std::string const &nickname);
		void		setUsername(std::string const &username);
		void		setRealname(std::string const &realname);
		void		setSentPassword(bool didSend);
		void		setReadBuffer(std::string const &buffer);
		void		setSendBuffer(std::string const &buffer);
		void		addSendBuffer(std::string buffer);

		/* Overloaded operators */
		User	&operator=(User const &origin);

};
#endif
