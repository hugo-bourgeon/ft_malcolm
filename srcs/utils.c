/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:27:44 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/14 18:38:37 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void	free_all(int exit_code, t_malcolm *malcolm)
{
	if (malcolm)
	{
		if (malcolm->src_ip)
			free(malcolm->src_ip);
		if (malcolm->src_mac)
			free(malcolm->src_mac);
		if (malcolm->trgt_ip)
			free(malcolm->trgt_ip);
		if (malcolm->trgt_mac)
			free(malcolm->trgt_mac);
		if (malcolm->sockfd > 0)
			close(malcolm->sockfd);
		if (malcolm->ifa_name)
			free(malcolm->ifa_name);
		
		free(malcolm);
	}
	if (exit_code == EXIT_FAILURE || exit_code == EXIT_SUCCESS)
		exit(exit_code);
}

void	print_arp_request(struct ether_arp *arp, char *ip_str)
{
	printf("An ARP request has been broadcast :\n");
	printf("    - Mac adress: %02x:%02x:%02x:%02x:%02x:%02x\n",
		arp->arp_sha[0],
		arp->arp_sha[1],
		arp->arp_sha[2],
		arp->arp_sha[3],
		arp->arp_sha[4],
		arp->arp_sha[5]);
	printf("    - IP adress:  %s\n", ip_str);
}
