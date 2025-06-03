/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:26:49 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/03 18:16:37 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void init(t_malcolm **malcolm)
{
	*malcolm = malloc(sizeof(t_malcolm));
	if (!(*malcolm))
	{
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, *malcolm);
	}

	ft_memset((*malcolm)->src_ip, 0, INET_ADDRSTRLEN);
	ft_memset((*malcolm)->src_mac, 0, MAC_ADDRSTRLEN);
	ft_memset((*malcolm)->trgt_ip, 0, INET_ADDRSTRLEN);
	ft_memset((*malcolm)->trgt_mac, 0, MAC_ADDRSTRLEN);
	ft_memset((*malcolm)->ifa_name, 0, MAC_ADDRSTRLEN);

	(*malcolm)->sockfd = 0;
	(*malcolm)->ifa_index = 0;
	(*malcolm)->verbose = 0;
	(*malcolm)->flood = 0;
}

void init_arp_socket(t_malcolm *malcolm, const char *iface_name)
{
	// Create a raw socket
	malcolm->sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (malcolm->sockfd < 0)
	{
		fprintf(stderr, "socket: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}

	// Bind socket to interface
	if (setsockopt(malcolm->sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface_name, strlen(iface_name)) < 0)
	{
		fprintf(stderr, "setsockopt(SO_BINDTODEVICE): %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}

	malcolm->ifa_index = if_nametoindex(iface_name);
	if (malcolm->ifa_index == 0)
	{
		fprintf(stderr, "if_nametoindex: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}
}
