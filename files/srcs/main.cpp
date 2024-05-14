/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 10:31:29 by acrespy           #+#    #+#             */
/*   Updated: 2024/05/14 10:36:10 by acrespy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./../includes/ft_irc.h"

int main(int argc, char **argv)
{
	try
	{
		Server serv(argc, argv);
		serv.start();
	}
	catch (std::exception &e)
	{
		ft_print(e.what(), ERROR);
		return (1);
	}

	return (0);
}
