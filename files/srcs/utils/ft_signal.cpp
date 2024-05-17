/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:20:26 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/17 16:20:26 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_irc.h"

/**
	* @brief	Signal handler
	* @param	int sig : Signal number
	* @param	t_siginfo siginfo : Sigaction struct
	* @param	void *content : Context
	* @return	None
	* @throw 	std::runtime_error If the signal is caught
*/
void ft_servSHandler(int sig, t_siginfo *siginfo, void *content)
{
	(void)sig;
	(void)siginfo;
	(void)content;

	std::cout << std::endl;
	throw std::runtime_error("Signal caught");

}
