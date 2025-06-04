/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:31:52 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/04 15:16:21 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void get_ip(char *src_ip, char *trgt_ip, t_malcolm *malcolm)
{
	if (!get_hostname(malcolm->src_ip, src_ip))
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: '%s'\n", src_ip);
		free_all(EXIT_FAILURE);
	}
	if (!get_hostname(malcolm->trgt_ip, trgt_ip))
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: '%s'\n", trgt_ip);
		free_all(EXIT_FAILURE);
	}
}

void get_mac(char *src_mac, char *trgt_mac, t_malcolm *malcolm)
{
	if (!is_valid_mac(src_mac))
	{
		fprintf(stderr, "ft_malcolm: invalid source mac address: '%s'\n", src_mac);
		free_all(EXIT_FAILURE);
	}
	if (!is_valid_mac(trgt_mac))
	{
		fprintf(stderr, "ft_malcolm: invalid target mac address: '%s'\n", trgt_mac);
		free_all(EXIT_FAILURE);
	}
	ft_memcpy(malcolm->src_mac, src_mac, MAC_ADDRSTRLEN);
	ft_memcpy(malcolm->trgt_mac, trgt_mac, MAC_ADDRSTRLEN);
}

void parsing(int argc, char **argv, t_malcolm *malcolm)
{
	int	  i = 1;
	int	  arg_count = 0;
	char *args[4] = { NULL };

	if (argc == 2 && ft_strncmp(argv[1], "--usage", ft_strlen(argv[1]) + 1) == 0)
	{
		printf("Usage: ./ft_malcolm [-v] [-f] [-i] <src_ip> <src_mac> <trgt_ip> <trgt_mac>\n");
		free_all(EXIT_SUCCESS);
	}

	// Parse optional flags
	while (i < argc && argv[i][0] == '-')
	{
		if (ft_strncmp(argv[i], "-v", 3) == 0)
			malcolm->verbose = 1;
		else if (ft_strncmp(argv[i], "-f", 3) == 0)
			malcolm->flood = 1;
		else if (ft_strncmp(argv[i], "-i", 3) == 0)
			malcolm->inspect = 1;
		else
		{
			fprintf(stderr, "./ft_malcolm: invalid option '%s'\n", argv[i]);
			fprintf(stderr, "Try './ft_malcolm --usage' for more information.\n");
			free_all(EXIT_FAILURE);
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
		free_all(EXIT_FAILURE);
	}

	get_ip(args[0], args[2], malcolm);
	get_mac(args[1], args[3], malcolm);
}
