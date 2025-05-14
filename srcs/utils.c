/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:27:44 by hubourge          #+#    #+#             */
/*   Updated: 2025/05/14 16:29:38 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malcolm.h"

void	free_all(int exit_code, t_malcolm *malcolm)
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
		
		free(malcolm);
	}
	if (exit_code == EXIT_FAILURE || exit_code == EXIT_SUCCESS)
		exit(exit_code);
}