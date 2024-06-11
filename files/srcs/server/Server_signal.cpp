/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_signal.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:37:20 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/11 11:37:20 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

/**
	*  @brief	Signal handler for SIGINT and SIGTERM
	*  @param	None
	*  @return	None
	*  @throw	std::runtime_error if the syscall sigaction() fails
*/
void Server::servSignal(void)
{
	_signal.sa_sigaction = ft_servSHandler;
	sigemptyset(&_signal.sa_mask);
	_signal.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT, &_signal, NULL) == -1
		|| sigaction(SIGTERM, &_signal, NULL) == -1)
		throw std::runtime_error("Syscall sigaction() Failed in servSignal: " + (std::string)std::strerror(errno));
}
