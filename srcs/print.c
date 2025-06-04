/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:55:05 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/04 14:51:17 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void print_info(t_malcolm *malcolm)
{
	printf(COLOR_GREEN "\n Verbose mode :             " COLOR_RESET COLOR_RED "%s\n" COLOR_RESET,
		   malcolm->verbose ? "ON" : "OFF");
	printf(COLOR_GREEN " Flooding mode :            " COLOR_RESET COLOR_RED "%s\n" COLOR_RESET,
		   malcolm->flood ? "ON" : "OFF");
	printf(COLOR_GREEN " Decimal notation mode :    " COLOR_RESET COLOR_RED "ALWAYS ON\n" COLOR_RESET);
	printf(COLOR_GREEN " Hostname resolution mode : " COLOR_RESET COLOR_RED "ALWAYS ON\n" COLOR_RESET);

	printf(COLOR_CYAN "\n Attack info :\n" COLOR_RESET);
	printf(COLOR_RED "           - source ip:             %s" COLOR_RESET COLOR_GREEN " (%u)\n" COLOR_RESET,
		   malcolm->src_ip, ip_to_decimal(malcolm->src_ip));
	printf(COLOR_RED "           - source mac (spoodef):  %s\n" COLOR_RESET, malcolm->src_mac);
	printf(COLOR_RED "           - target ip:             %s" COLOR_RESET COLOR_GREEN " (%u)\n" COLOR_RESET,
		   malcolm->trgt_ip, ip_to_decimal(malcolm->trgt_ip));
	printf(COLOR_RED "           - target mac:            %s\n\n" COLOR_RESET, malcolm->trgt_mac);
}

void print_sending(t_malcolm *malcolm)
{
	printf("\n Sending an " COLOR_CYAN "ARP reply" COLOR_RESET " to " COLOR_RED "%s (target)" COLOR_RESET
		   " with " COLOR_RED "spoofed mac" COLOR_RESET ", wait...\n",
		   malcolm->trgt_ip);
}

void print_sent(t_malcolm *malcolm, uint8_t src_mac[6], unsigned char *packet, ssize_t len)
{
	printf(" Sent an " COLOR_CYAN "ARP reply" COLOR_RESET " packet: " COLOR_RED
		   "%s is-at %02x:%02x:%02x:%02x:%02x:%02x" COLOR_RESET "\n",
		   malcolm->src_ip, src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);
	if (malcolm->verbose)
	{
		print_hexdump(packet, len);
		printf("\n");
	}
}

void print_exit(void)
{
	printf("\n Please check " COLOR_CYAN "Arp table" COLOR_RESET " on the " COLOR_RED "target\n" COLOR_RESET);
	printf(" Exiting program...\n\n");
}

void print_hexdump(unsigned char *buffer, ssize_t len)
{
	printf(COLOR_GREEN " Hexdump :" COLOR_RESET "\n");
	for (int i = 0; i < len; i++)
	{
		if (i % 16 == 0)
			printf("      %04x: ", i);
		printf("%02x ", buffer[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	if (len % 16 != 0)
		printf("\n");
}

void print_arp_request(t_malcolm *malcolm, struct ether_arp *arp, struct ether_header *eth, char *ip_str, ssize_t len,
					   unsigned char buffer[MAX_BUFFER_SIZE])
{
	if (malcolm->verbose)
	{
		printf(COLOR_CYAN "\n An ARP request has been broadcast from target :\n" COLOR_RESET);
		printf(COLOR_GREEN "\n =================== Verbose mode ==================\n" COLOR_RESET);
		print_hexdump(buffer, len);

		printf(COLOR_GREEN "\n Ethernet II :" COLOR_RESET "\n");
		printf("      Destination: %02x:%02x:%02x:%02x:%02x:%02x (Broadcast)\n", eth->ether_dhost[0],
			   eth->ether_dhost[1], eth->ether_dhost[2], eth->ether_dhost[3], eth->ether_dhost[4], eth->ether_dhost[5]);
		printf("      Source:      %02x:%02x:%02x:%02x:%02x:%02x\n", eth->ether_shost[0], eth->ether_shost[1],
			   eth->ether_shost[2], eth->ether_shost[3], eth->ether_shost[4], eth->ether_shost[5]);
		printf("      Type:        ARP (0x%04x)\n", ntohs(eth->ether_type));

		printf(COLOR_GREEN "\n Address Resolution Protocol (ARP request) :" COLOR_RESET "\n");
		printf("      Hardware type: Ethernet (1)\n");
		printf("      Protocol type: IPv4 (0x%04x)\n", ntohs(arp->ea_hdr.ar_pro));
		printf("      Hardware size: %d\n", arp->ea_hdr.ar_hln);
		printf("      Protocol size: %d\n", arp->ea_hdr.ar_pln);
		printf("      Opcode:        request (%d)\n", ntohs(arp->ea_hdr.ar_op));
		printf("      Sender MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n", arp->arp_sha[0], arp->arp_sha[1],
			   arp->arp_sha[2], arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
		printf("      Sender IP address:  %s\n", ip_str);
		printf("      Target MAC address: 00:00:00:00:00:00 (unknown)\n");
		printf("      Target IP address:  %u.%u.%u.%u\n", arp->arp_tpa[0], arp->arp_tpa[1], arp->arp_tpa[2],
			   arp->arp_tpa[3]);
		printf(COLOR_GREEN " ====================================================\n" COLOR_RESET);
	}
	else
	{
		printf(COLOR_CYAN "\n An ARP request has been broadcast from target :\n" COLOR_RESET);
		printf("     - Mac adress: " COLOR_RESET);
		printf(COLOR_RED "%02x:%02x:%02x:%02x:%02x:%02x\n" COLOR_RESET, arp->arp_sha[0], arp->arp_sha[1],
			   arp->arp_sha[2], arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
		printf("     - IP adress: ");
		printf(COLOR_RED " %s\n" COLOR_RESET, ip_str);
	}
}
