/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:15:25 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/04 16:02:15 by hubourge         ###   ########.fr       */
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
		free_all(EXIT_FAILURE);
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;

		// AF_PACKET only
		if (ifa->ifa_addr->sa_family == AF_PACKET && (ifa->ifa_flags & IFF_LOOPBACK) == 0 && (ifa->ifa_flags & IFF_UP))
		{
			if (malcolm->ifa_name[0] == '\0')
				printf(COLOR_CYAN " Found available interface : \n" COLOR_RESET);

			printf(COLOR_RED "           - %s\n" COLOR_RESET, ifa->ifa_name);
			ft_memcpy(malcolm->ifa_name, ifa->ifa_name, IF_NAMESIZE);

			malcolm->ifa_index = if_nametoindex(ifa->ifa_name);
			if (malcolm->ifa_index == 0)
			{
				fprintf(stderr, "if_nametoindex: %s\n", strerror(errno));
				free_all(EXIT_FAILURE);
			}
			break;
		}
	}

	freeifaddrs(ifaddr);
}

void listen_arp_requests(t_malcolm *malcolm, int sockfd)
{
	unsigned char	   buffer[MAX_BUFFER_SIZE];
	struct sockaddr_ll addr;
	socklen_t		   addr_len = sizeof(addr);

	signal(SIGINT, handle_sigint);
	printf(COLOR_CYAN "\n Listening for ARP requests on '%s'\n" COLOR_RESET, malcolm->ifa_name);

	while (1)
	{
		ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addr_len);
		if (len < 0)
		{
			fprintf(stderr, "recvfrom: %s\n", strerror(errno));
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

		// Display each ARP packet received in tcpdump style
		if (malcolm->inspect)
		{
			printf("\n");
			char sender_ip[INET_ADDRSTRLEN];
			char target_ip[INET_ADDRSTRLEN];
			sprintf(sender_ip, "%u.%u.%u.%u", arp->arp_spa[0], arp->arp_spa[1], arp->arp_spa[2], arp->arp_spa[3]);
			sprintf(target_ip, "%u.%u.%u.%u", arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2], arp->arp_tpa[3]);

			char *sender_hostname = resolve_hostname(sender_ip);
			char *target_hostname = resolve_hostname(target_ip);

			if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST)
				printf(COLOR_GREEN " ARP, Request who-has %s tell %s, length %zd\n" COLOR_RESET, target_hostname,
					   sender_hostname, len - sizeof(struct ether_header));
			else if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REPLY)
				printf(COLOR_GREEN " ARP, Reply %s is-at %02x:%02x:%02x:%02x:%02x:%02x, length %zd\n" COLOR_RESET,
					   sender_hostname, arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2], arp->arp_sha[3],
					   arp->arp_sha[4], arp->arp_sha[5], len - sizeof(struct ether_header));

			print_hexdump(buffer, len);
		}

		// Check ARP request
		if (ntohs(arp->ea_hdr.ar_op) != ARPOP_REQUEST)
			continue;

		// Extract sender IP
		char		   ip_str_src[INET_ADDRSTRLEN];
		unsigned char *ip = arp->arp_spa;

		// Check if the sender IP matches the target IP
		sprintf(ip_str_src, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
		if (ft_strncmp(ip_str_src, malcolm->trgt_ip, INET_ADDRSTRLEN) != 0)
			continue;

		// Extract target IP
		char		   ip_str_trgt[INET_ADDRSTRLEN];
		unsigned char *trgt_ip = arp->arp_tpa;
		sprintf(ip_str_trgt, "%u.%u.%u.%u", trgt_ip[0], trgt_ip[1], trgt_ip[2], trgt_ip[3]);

		// Check if the target IP matches the source
		if (ft_strncmp(ip_str_trgt, malcolm->src_ip, INET_ADDRSTRLEN) != 0)
			continue;

		print_arp_request(malcolm, arp, eth, ip_str_src);
		break;
	}
}

void send_arp_reply(t_malcolm *malcolm, int sockfd)
{
	unsigned char packet[PACKET_SIZE];

	// === Ethernet Header ===
	unsigned char *eth = packet;
	unsigned char *arp = packet + 14;

	// Destination MAC
	sscanf(malcolm->trgt_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth[0], &eth[1], &eth[2], &eth[3], &eth[4], &eth[5]);

	// Source MAC
	sscanf(malcolm->src_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth[6], &eth[7], &eth[8], &eth[9], &eth[10], &eth[11]);

	eth[12] = 0x08; // ARP type
	eth[13] = 0x06;

	// === ARP Header ===
	ft_memset(arp, 0, 28);
	arp[0] = 0x00;
	arp[1] = 0x01; // Ethernet
	arp[2] = 0x08;
	arp[3] = 0x00; // IPv4
	arp[4] = 6;	   // Hardware size
	arp[5] = 4;	   // Protocol size
	arp[6] = 0x00;
	arp[7] = 0x02; // ARP REPLY

	// Sender MAC
	sscanf(malcolm->src_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &arp[8], &arp[9], &arp[10], &arp[11], &arp[12], &arp[13]);

	// Sender IP
	inet_pton(AF_INET, malcolm->src_ip, &arp[14]);

	// Target MAC
	sscanf(malcolm->trgt_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &arp[18], &arp[19], &arp[20], &arp[21], &arp[22],
		   &arp[23]);

	// Target IP
	inet_pton(AF_INET, malcolm->trgt_ip, &arp[24]);

	// Set interface
	struct sockaddr_ll sll;
	ft_memset(&sll, 0, sizeof(struct sockaddr_ll));
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = malcolm->ifa_index;
	sll.sll_halen = ETH_ALEN;
	uint8_t dst_mac[6];
	parse_mac(malcolm->trgt_mac, dst_mac);
	ft_memcpy(sll.sll_addr, dst_mac, 6);

	uint8_t src_mac[6];
	parse_mac(malcolm->src_mac, src_mac);
	print_sending(malcolm);

	sleep(1);
	while (1)
	{
		if (sendto(sockfd, packet, PACKET_SIZE, 0, (struct sockaddr *)&sll, sizeof(sll)) < 0)
		{
			perror("sendto");
			return;
		}
		print_sent(malcolm, src_mac, packet, PACKET_SIZE);
		if (malcolm->flood == 0)
			break;
		sleep(1);
	}
}
