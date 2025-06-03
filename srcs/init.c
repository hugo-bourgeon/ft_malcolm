/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:26:49 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/03 18:36:47 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void init(t_malcolm *malcolm)
{
	ft_memset(malcolm->src_ip, 0, INET_ADDRSTRLEN);
	ft_memset(malcolm->src_mac, 0, MAC_ADDRSTRLEN);
	ft_memset(malcolm->trgt_ip, 0, INET_ADDRSTRLEN);
	ft_memset(malcolm->trgt_mac, 0, MAC_ADDRSTRLEN);
	ft_memset(malcolm->ifa_name, 0, MAC_ADDRSTRLEN);

	malcolm->ifa_index = 0;
	malcolm->verbose = 0;
	malcolm->flood = 0;
}

void init_arp_socket(t_malcolm *malcolm, const char *iface_name)
{
	// Create a raw socket
	g_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (g_socket < 0)
	{
		fprintf(stderr, "socket: %s\n", strerror(errno));
		free_all(EXIT_FAILURE);
	}

	// Bind socket to interface
	if (setsockopt(g_socket, SOL_SOCKET, SO_BINDTODEVICE, iface_name, strlen(iface_name)) < 0)
	{
		fprintf(stderr, "setsockopt(SO_BINDTODEVICE): %s\n", strerror(errno));
		free_all(EXIT_FAILURE);
	}

	malcolm->ifa_index = if_nametoindex(iface_name);
	if (malcolm->ifa_index == 0)
	{
		fprintf(stderr, "if_nametoindex: %s\n", strerror(errno));
		free_all(EXIT_FAILURE);
	}
}
