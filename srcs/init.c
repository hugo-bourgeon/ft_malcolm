/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:26:49 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/14 16:40:14 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void	init(t_malcolm **malcolm)
{
	*malcolm = malloc(sizeof(t_malcolm));
	if (!(*malcolm))
	{
		fprintf(stderr, "sendto error: %s\n", strerror(errno));
		free_all(EXIT_FAILURE, *malcolm);
	}

    (*malcolm)->src_ip = NULL;
    (*malcolm)->src_mac = NULL;
    (*malcolm)->trgt_ip = NULL;
    (*malcolm)->trgt_mac = NULL;
}
