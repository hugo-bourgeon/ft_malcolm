/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:36:01 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/21 18:58:27 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

int g_stop_code = PROCESS;

void print_struct(t_malcolm *malcolm);

int main(int argc, char **argv)
{
	t_malcolm *malcolm;

	init(&malcolm);
	parsing(argc, argv, malcolm);
	print_struct(malcolm);

	get_arp_interfaces(malcolm);
	init_arp_socket(malcolm, malcolm->ifa_name);

	listen_arp_requests(malcolm, malcolm->sockfd);
	send_arp_reply(malcolm, malcolm->sockfd);

	print_exit();
	free_all(NOT_EXIT, malcolm);
	return (0);
}

void print_struct(t_malcolm *malcolm)
{
	printf("------ Struct ------\n");
	printf("|   src_ip:   %s\n", malcolm->src_ip);
	printf("|   src_mac:  %s\n", malcolm->src_mac);
	printf("|   trgt_ip:  %s\n", malcolm->trgt_ip);
	printf("|   trgt_mac: %s\n", malcolm->trgt_mac);
	printf("--------------------\n");
}
