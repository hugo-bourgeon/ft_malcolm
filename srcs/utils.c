/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:27:44 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/03 18:10:43 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void free_all(int exit_code, t_malcolm *malcolm)
{
	if (malcolm)
	{
		// if (malcolm->src_ip)
		// 	free(malcolm->src_ip);
		// if (malcolm->src_mac)
		// 	free(malcolm->src_mac);
		// if (malcolm->trgt_ip)
		// 	free(malcolm->trgt_ip);
		// if (malcolm->trgt_mac)
		// 	free(malcolm->trgt_mac);
		if (malcolm->sockfd > 0)
			close(malcolm->sockfd);
		// if (malcolm->ifa_name)
		// 	free(malcolm->ifa_name);

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
	exit(1);
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

int is_valid_mac(const char *mac)
{
	int i = 0;
	int count = 0;

	while (mac[i])
	{
		if (mac[i] == ':')
			count++;
		else if (!((mac[i] >= '0' && mac[i] <= '9') || (mac[i] >= 'a' && mac[i] <= 'f')
				   || (mac[i] >= 'A' && mac[i] <= 'F')))
			return 0;
		i++;
	}
	if (count == 5 && i == 17)
		return (1);
	return (0);
}

void parse_mac(const char *str, uint8_t mac[6])
{
	for (int i = 0; i < 6; i++)
	{
		mac[i] = (hexchar_to_int(str[0]) << 4) | hexchar_to_int(str[1]);
		str += 3; // skip ':'
	}
}

int resolve_hostname(char ip[INET_ADDRSTRLEN], const char *hostname)
{
	struct addrinfo hints, *res;
	char			ipstr[INET_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;

	if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
		return (0);

	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, INET_ADDRSTRLEN);
	freeaddrinfo(res);

	ft_memcpy(ip, ipstr, INET_ADDRSTRLEN);
	return (1);
}
