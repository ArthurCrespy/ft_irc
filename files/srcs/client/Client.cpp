/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:49:33 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/12 13:54:56 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

Client::Client(void) : _cli_fd(-1), _cli_port(-1), _cli_p(false), _cli_n(false), _cli_u(false), _cli_l(false) {}

Client::Client(int cli_fd, int cli_port, std::string const &hostname) : _cli_fd(cli_fd), _cli_port(cli_port), _cli_p(false), _cli_n(false), _cli_u(false), _cli_l(false), _cli_hostname(hostname) {}

Client::Client(Client const &src)
{
	*this = src;
}

Client::~Client(void) {}

Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		this->_cli_fd = rhs._cli_fd;
		this->_cli_port = rhs._cli_port;
		this->_cli_p = rhs._cli_p;
		this->_cli_n = rhs._cli_n;
		this->_cli_u = rhs._cli_u;
		this->_cli_l = rhs._cli_l;
		this->_cli_nickname = rhs._cli_nickname;
		this->_cli_username = rhs._cli_username;
		this->_cli_realname = rhs._cli_realname;
		this->_cli_hostname = rhs._cli_hostname;
	}
	return (*this);
}
