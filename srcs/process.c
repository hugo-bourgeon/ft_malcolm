/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:15:25 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/19 15:52:28 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void get_arp_interfaces(t_malcolm *malcolm)
{
	struct ifaddrs *ifaddr, *ifa;

	// list of network interfaces
	if (getifaddrs(&ifaddr) == -1)
	{
		fprintf(stderr, "socket: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;

		// AF_PACKET only
		if (ifa->ifa_addr->sa_family == AF_PACKET \
			&& (ifa->ifa_flags & IFF_LOOPBACK) == 0 \
			&& (ifa->ifa_flags & IFF_UP))
		{
			printf("Found available interface : \n");
			printf("    - %s\n", ifa->ifa_name);
			malcolm->ifa_name = ft_strdup(ifa->ifa_name);
		}
	}

	freeifaddrs(ifaddr);
}

void	listen_arp_requests(t_malcolm *malcolm, int sockfd)
{
	unsigned char buffer[MAX_BUFFER_SIZE];
	signal(SIGINT, handle_sigint);

	while (1)
	{
		check_sigint(malcolm);
		
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(sockfd, &read_fds);

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int ret = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
		if (ret < 0)
		{
			check_sigint(malcolm);
			fprintf(stderr, "select: %s\n", strerror(errno));
			break;
		}
		else if (ret == 0)
			continue;
	
		ssize_t len = recv(sockfd, buffer, sizeof(buffer), 0);
		if (len < 0)
		{
			fprintf(stderr, "recv: %s\n", strerror(errno));
			continue;
		}

		struct ether_header *eth = (struct ether_header *)buffer;

		// ARP packets only
		if (ntohs(eth->ether_type) != ETH_P_ARP)
			continue;

		// Check broadcast
		if (ft_memcmp(eth->ether_dhost, "\xff\xff\xff\xff\xff\xff", 6) != 0)
			continue;

		struct ether_arp *arp = (struct ether_arp *)(buffer + sizeof(struct ether_header));

		// Check ARP request
		if (ntohs(arp->ea_hdr.ar_op) != ARPOP_REQUEST)
			continue;

		// Extract sender MAC/IP
		char			ip_str[INET_ADDRSTRLEN];
		unsigned char	*ip = arp->arp_spa;

		// Convert IP to string
		sprintf(ip_str, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
		print_arp_request(arp, ip_str);

		// break;
	}
}
