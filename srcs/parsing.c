/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:31:52 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/22 15:22:26 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

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

	malcolm->src_mac = dup_str(malcolm, argv[2]);
	malcolm->trgt_mac = dup_str(malcolm, argv[4]);

	malcolm->src_ip = resolve_hostname(malcolm, argv[1]);
	malcolm->trgt_ip = resolve_hostname(malcolm, argv[3]);
	if (!malcolm->src_ip)
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: '%s'\n", argv[1]);
		free_all(EXIT_FAILURE, malcolm);
	}
	if (!malcolm->trgt_ip)
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: '%s'\n", argv[3]);
		free_all(EXIT_FAILURE, malcolm);
	}
}