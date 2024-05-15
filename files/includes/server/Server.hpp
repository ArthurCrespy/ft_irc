/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:28:57 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/14 14:28:57 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

typedef struct sockaddr_in t_sockaddr_in;
typedef struct pollfd t_pollfd;
typedef socklen_t t_socklen;
typedef std::vector<pollfd>::iterator it_pollfds;

class Server
{
	private:
		int				_port;
		std::string		_password;

        int				_srv_sock;
		int 			_srv_opt;
		t_sockaddr_in	_srv_sock_adrr;
		t_pollfd 		_srv_poll;


        std::vector<pollfd>     _poll_fds;
		// todo: maybe do a map of fd to client
		// std::map<pollfd, Client>	_clients;

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
		void		servClose(int fd);
		void		servReceive(int fd);

		void		setPort(int port);
		void		setPort(std::string const &input);
		void		setPassword(std::string const &input);

		int			getPort(void) const;
		std::string	getPassword(void) const;
};


#endif
