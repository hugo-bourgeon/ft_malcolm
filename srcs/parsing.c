/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:31:52 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/14 16:40:26 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void	parsing(int argc, char **argv, t_malcolm *malcolm)
{
	if (argc != 5)
	{
		fprintf(stderr, "./ft_malcolm: missing host operand\n");
		fprintf(stderr, "Try './ft_malcolm --usage' for more information.\n");
		free_all(EXIT_FAILURE, malcolm);
	}

	malcolm->src_ip = strdup(argv[1]);
	if (!malcolm->src_ip)
	{
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}
	malcolm->src_mac = strdup(argv[2]);
	if (!malcolm->src_mac)
	{
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}
	malcolm->trgt_ip = strdup(argv[3]);
	if (!malcolm->trgt_ip)
	{
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}
	malcolm->trgt_mac = strdup(argv[4]);
	if (!malcolm->trgt_mac)
	{
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}
}