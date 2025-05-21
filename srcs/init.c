/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:26:49 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/21 19:19:58 by hubourge         ###   ########.fr       */
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

	(*malcolm)->src_ip = NULL;
	(*malcolm)->src_mac = NULL;
	(*malcolm)->trgt_ip = NULL;
	(*malcolm)->trgt_mac = NULL;
	(*malcolm)->sockfd = 0;
	(*malcolm)->ifa_name = NULL;
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
	if (setsockopt(malcolm->sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface_name,
				   strlen(iface_name))
		< 0)
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
