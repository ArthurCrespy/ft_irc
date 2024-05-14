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

class Server
{
	private:
        int				_sock;

		int 			_opt;
		t_sockaddr_in	_sock_adrr;

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

		void		serv_socket(void);
		void		serv_poll(void);

		void		setPort(int port);
		void		setPort(std::string const &input);
		void		setPassword(std::string const &input);

		int			getPort(void) const;
		std::string	getPassword(void) const;
};


#endif
