/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 16:16:16 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/31 16:17:16 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_queues(t_settings *settings, t_dongle *dongles)
{
	int	i;

	i = 0;
	while (i < settings->number_of_coders)
	{
		free(dongles[i].queue->array);
		free(dongles[i].queue);
		i++;
	}
}
