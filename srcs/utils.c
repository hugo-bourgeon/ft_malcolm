/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:27:44 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/14 17:59:35 by hubourge         ###   ########.fr       */
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
			printf("Found available interface: %s\n", ifa->ifa_name);
			malcolm->ifa_name = ft_strdup(ifa->ifa_name);
		}
	}

	freeifaddrs(ifaddr);
}