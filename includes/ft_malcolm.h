/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:37:35 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/14 17:51:50 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <sys/socket.h>
# include <linux/if_packet.h>
# include <net/ethernet.h>
# include <netinet/if_ether.h>
# include <net/if.h>
# include <sys/ioctl.h>
# include <arpa/inet.h>
# include <ifaddrs.h>
# include <sys/types.h>


# define NOT_EXIT		-1
# define PROCESS		0
# define STOP			1

extern int	g_stop_code;

typedef struct s_malcolm
{
	char    *src_ip;
	char    *src_mac;
	char    *trgt_ip;
	char    *trgt_mac;
	int     sockfd;
	char	*ifa_name;
}	t_malcolm;


// init.c
void	init(t_malcolm **malcolm);
void	init_arp_socket(t_malcolm *malcolm, const char *iface_name);

// parsing.c
void	parsing(int argc, char **argv, t_malcolm *malcolm);

// utils.c
void	free_all(int exit_code, t_malcolm *malcolm);
void	get_arp_interfaces(t_malcolm *malcolm);

#endif
