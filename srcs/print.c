/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:55:05 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/22 15:23:18 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void print_info(t_malcolm *malcolm)
{
	printf(COLOR_CYAN "\n Spoofing info :\n" COLOR_RESET);
	printf(COLOR_RED "           - source ip:             %s\n" COLOR_RESET, malcolm->src_ip);
	printf(COLOR_RED "           - source mac (spoodef):  %s\n" COLOR_RESET, malcolm->src_mac);
	printf(COLOR_RED "           - target ip:             %s\n" COLOR_RESET, malcolm->trgt_ip);
	printf(COLOR_RED "           - target mac:            %s\n\n" COLOR_RESET, malcolm->trgt_mac);
}

void print_arp_request(struct ether_arp *arp, char *ip_str)
{
	printf(COLOR_CYAN "\n An ARP request has been broadcast :\n" COLOR_RESET);
	printf("     - Mac adress: " COLOR_RESET);
	printf(COLOR_RED "%02x:%02x:%02x:%02x:%02x:%02x\n" COLOR_RESET, arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2],
		   arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
	printf("     - IP adress: ");
	printf(COLOR_RED " %s\n" COLOR_RESET, ip_str);
}

void print_sending(t_malcolm *malcolm)
{
	printf("\n Sending an " COLOR_CYAN "ARP reply" COLOR_RESET " to " COLOR_RED "%s (target)" COLOR_RESET
		   " with " COLOR_RED "spoofed mac" COLOR_RESET ", wait...\n",
		   malcolm->trgt_ip);
}

void print_sent(t_malcolm *malcolm, uint8_t src_mac[6])
{
	printf(" Sent an " COLOR_CYAN "ARP reply" COLOR_RESET " packet: " COLOR_RED
		   "%s is-at %02x:%02x:%02x:%02x:%02x:%02x" COLOR_RESET "\n",
		   malcolm->src_ip, src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);
}

void print_check(void)
{
	printf("\n Please check " COLOR_CYAN "Arp table" COLOR_RESET " on the " COLOR_RED "target\n" COLOR_RESET);
}

void print_exit(void)
{
	printf(" Exiting program...\n\n");
}
