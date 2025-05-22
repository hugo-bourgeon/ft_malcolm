/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:31:52 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/22 15:37:41 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void get_ip(char *src_ip, char *trgt_ip, t_malcolm *malcolm)
{
	malcolm->src_ip = resolve_hostname(malcolm, src_ip);
	malcolm->trgt_ip = resolve_hostname(malcolm, trgt_ip);
	if (!malcolm->src_ip)
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: '%s'\n", src_ip);
		free_all(EXIT_FAILURE, malcolm);
	}
	if (!malcolm->trgt_ip)
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: '%s'\n", trgt_ip);
		free_all(EXIT_FAILURE, malcolm);
	}
}

void get_mac(char *src_mac, char *trgt_mac, t_malcolm *malcolm)
{
	if (!is_valid_mac(src_mac))
	{
		fprintf(stderr, "ft_malcolm: invalid source mac address: '%s'\n", src_mac);
		free_all(EXIT_FAILURE, malcolm);
	}
	if (!is_valid_mac(trgt_mac))
	{
		fprintf(stderr, "ft_malcolm: invalid target mac address: '%s'\n", trgt_mac);
		free_all(EXIT_FAILURE, malcolm);
	}

	malcolm->src_mac = dup_str(malcolm, src_mac);
	malcolm->trgt_mac = dup_str(malcolm, trgt_mac);
}

void parsing(int argc, char **argv, t_malcolm *malcolm)
{
	if (argc == 2 && strncmp(argv[1], "--usage", ft_strlen(argv[1]) + 1) == 0)
	{
		printf("Usage: ./ft_malcolm <src_ip> <src_mac> <trgt_ip> <trgt_mac>\n");
		free_all(EXIT_SUCCESS, malcolm);
	}

	if (argc != 5)
	{
		fprintf(stderr, "./ft_malcolm: missing host operand\n");
		fprintf(stderr, "Try './ft_malcolm --usage' for more information.\n");
		free_all(EXIT_FAILURE, malcolm);
	}

	get_ip(argv[1], argv[3], malcolm);
	get_mac(argv[2], argv[4], malcolm);
}