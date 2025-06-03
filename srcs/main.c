/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:36:01 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/03 18:42:03 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int g_socket = 0;

int main(int argc, char **argv)
{
	t_malcolm malcolm;

	init(&malcolm);
	parsing(argc, argv, &malcolm);
	print_info(&malcolm);

	get_arp_interfaces(&malcolm);
	init_arp_socket(&malcolm, malcolm.ifa_name);

	listen_arp_requests(&malcolm, g_socket);
	send_arp_reply(&malcolm, g_socket);

	print_exit();
	free_all(NOT_EXIT);
	return (0);
}
