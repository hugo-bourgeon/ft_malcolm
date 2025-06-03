/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:36:01 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/03 14:54:10 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int g_stop_code = PROCESS;

int main(int argc, char **argv)
{
	t_malcolm *malcolm;

	init(&malcolm);
	parsing(argc, argv, malcolm);
	print_info(malcolm);

	get_arp_interfaces(malcolm);
	init_arp_socket(malcolm, malcolm->ifa_name);

	listen_arp_requests(malcolm, malcolm->sockfd);
	send_arp_reply(malcolm, malcolm->sockfd);

	print_exit();
	free_all(NOT_EXIT, malcolm);
	return (0);
}
