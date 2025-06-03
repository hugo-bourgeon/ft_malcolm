/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:27:44 by hubourge          #+#    #+#             */
/*   Updated: 2025/06/03 18:42:21 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void free_all(int exit_code)
{
	if (g_socket > 0)
		close(g_socket);
	if (exit_code == EXIT_FAILURE || exit_code == EXIT_SUCCESS)
		exit(exit_code);
}

void handle_sigint(int sig)
{
	(void)sig;
	if (g_socket > 0)
		close(g_socket);
	print_exit();
	exit(1);
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
