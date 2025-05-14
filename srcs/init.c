/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:26:49 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/14 18:38:14 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void	init(t_malcolm **malcolm)
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

void	init_arp_socket(t_malcolm *malcolm, const char *iface_name)
{
	struct sockaddr_ll sll;

	// Create a raw socket to capture ARP packets
	malcolm->sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (malcolm->sockfd < 0)
	{
		fprintf(stderr, "socket: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}

	// Get interface index
	unsigned int ifindex = if_nametoindex(iface_name);
	if (ifindex == 0)
	{
		fprintf(stderr, "if_nametoindex: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}

	// Set sockaddr_ll, bind the socket to the interface
	memset(&sll, 0, sizeof(sll));
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifindex;
	sll.sll_protocol = htons(ETH_P_ARP);

	if (bind(malcolm->sockfd, (struct sockaddr *)&sll, sizeof(sll)) < 0)
	{
        fprintf(stderr, "bind: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}
}
