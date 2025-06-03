/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:31:52 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/03 16:08:21 by hubourge         ###   ########.fr       */
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
	int	  i = 1;
	int	  arg_count = 0;
	char *args[4] = { NULL };

	if (argc == 2 && ft_strncmp(argv[1], "--usage", ft_strlen(argv[1]) + 1) == 0)
	{
		printf("Usage: ./ft_malcolm [-v] [-f] <src_ip> <src_mac> <trgt_ip> <trgt_mac>\n");
		free_all(EXIT_SUCCESS, malcolm);
	}

	// Parse optional flags
	while (i < argc && argv[i][0] == '-')
	{
		if (ft_strncmp(argv[i], "-v", 3) == 0)
			malcolm->verbose = 1;
		else if (ft_strncmp(argv[i], "-f", 3) == 0)
			malcolm->flood = 1;
		else
		{
			fprintf(stderr, "./ft_malcolm: invalid option '%s'\n", argv[i]);
			fprintf(stderr, "Try './ft_malcolm --usage' for more information.\n");
			free_all(EXIT_FAILURE, malcolm);
		}
		i++;
	}

	// Collect remaining arguments
	while (i < argc && arg_count < 4)
	{
		args[arg_count] = argv[i];
		arg_count++;
		i++;
	}

	if (arg_count < 4)
	{
		fprintf(stderr, "./ft_malcolm: missing host operand\n");
		fprintf(stderr, "Try './ft_malcolm --usage' for more information.\n");
		free_all(EXIT_FAILURE, malcolm);
	}

	get_ip(args[0], args[2], malcolm);
	get_mac(args[1], args[3], malcolm);
}