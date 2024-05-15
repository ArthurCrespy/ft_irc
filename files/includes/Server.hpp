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
        int				_sock_srv;
		int 			_opt_srv;
		t_sockaddr_in	_sock_adrr_srv;
		t_pollfd 		_poll_srv;

        std::vector<pollfd>     _poll_fds;

		int				_port;
		std::string	_password;




	public:
		Server(void);
		Server(int argc, char **argv);
		Server(int port, std::string name);
		Server(Server const &src);
		~Server(void);

		Server &operator=(Server const &rhs);

		void		start(void);

		void		setup(int argc, char **argv);

		void		servSocket(void);
		void		servPoll(void);
		void		servAccept(void);

		void		setPort(int port);
		void		setPort(std::string const &input);
		void		setPassword(std::string const &input);

		int			getPort(void) const;
		std::string	getPassword(void) const;
};


#endif
