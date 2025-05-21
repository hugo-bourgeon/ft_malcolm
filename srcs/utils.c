/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:27:44 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/21 19:19:56 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void free_all(int exit_code, t_malcolm *malcolm)
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

char *dup_str(t_malcolm *malcolm, const char *str)
{
	char *dup;

	dup = ft_strdup(str);
	if (!dup)
	{
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, malcolm);
	}
	return (dup);
}

void handle_sigint(int sig)
{
	(void)sig;
	g_stop_code = STOP;
}

void check_sigint(t_malcolm *malcolm)
{
	if (g_stop_code == STOP)
		free_all(EXIT_SUCCESS, malcolm);
}

int hexchar_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return (-1); // Should not happen
}

void parse_mac(const char *str, uint8_t mac[6])
{
	for (int i = 0; i < 6; i++)
	{
		mac[i] = (hexchar_to_int(str[0]) << 4) | hexchar_to_int(str[1]);
		str += 3; // skip ':'
	}
}
