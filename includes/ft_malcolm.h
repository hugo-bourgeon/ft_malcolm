/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:37:35 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/21 19:19:43 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include "../libft/libft.h"
#include <arpa/inet.h>
#include <errno.h>
#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define NOT_EXIT -1
#define PROCESS 0
#define STOP 1

#define MAX_BUFFER_SIZE 65536

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN "\033[1;36m"

extern int g_stop_code;

typedef struct s_malcolm
{
	char *src_ip;
	char *src_mac;
	char *trgt_ip;
	char *trgt_mac;
	int	  sockfd;
	char *ifa_name;
	int	  ifa_index;
} t_malcolm;

// process.c
void get_arp_interfaces(t_malcolm *malcolm);
void listen_arp_requests(t_malcolm *malcolm, int sockfd);
void send_arp_reply(t_malcolm *malcolm, int sockfd);
void send_fake_arp_request(t_malcolm *malcolm, const char *src_ip,
						   const char *trgt_ip, const char *spoof_mac);

// init.c
void init(t_malcolm **malcolm);
void init_arp_socket(t_malcolm *malcolm, const char *iface_name);

// parsing.c
void parsing(int argc, char **argv, t_malcolm *malcolm);

// utils.c
void  free_all(int exit_code, t_malcolm *malcolm);
char *dup_str(t_malcolm *malcolm, const char *str);
void  handle_sigint(int sig);
void  check_sigint(t_malcolm *malt_malcolm);
int	  hexchar_to_int(char c);
void  parse_mac(const char *str, uint8_t mac[6]);

// print.c
void print_arp_request(struct ether_arp *arp, char *ip_str);
void print_sending(t_malcolm *malcolm);
void print_sent(t_malcolm *malcolm, uint8_t src_mac[6]);
void print_check(void);
void print_exit(void);

#endif
