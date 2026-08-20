/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:35:34 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/20 17:37:34 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void * arg)
{
	t_program	*simulation;

	simulation = (t_program *)arg;

	while (1)
	{
		if (burnout_detected(simulation))
			break ;
	}
	return (NULL);
}

int	burnout_detected(t_program *simulation)
{
	int		i;
	long	time_in_ms;

	time_in_ms = get_timestamp(simulation->start);
	i = 0;
	while (i < simulation->settings.number_of_coders)
	{
		if (time_in_ms > simulation->coders[i].burnout)
		{
			pthread_mutex_lock(&simulation->mutex_print);
			printf("%ld %d burned out\n", time_in_ms, simulation->coders[i].id);
			pthread_cond_broadcast(&simulation->cond_monitor);
			pthread_mutex_unlock(&simulation->mutex_print);
			return (1);
		}
		i++;
	}
	return (0);
}

int	find_nearest_burnout(t_program *simulation)
{
	int		i;
	int		nearest_deadline;
	long	time_in_ms;

	time_in_ms = get_timestamp(simulation->start);
	i = 0;
	nearest_deadline = simulation->coders[0]->begin_of_last_compile + simulation->settings.time_to_burnout;
	while (i < simulation->settings.number_of_coders)
	{
		coder_deadline = simulation->coders[i]->begin_of_last_compile + simulation->settings.time_to_burnout;
		if (coder_deadline < nearest_deadline)
		{
			nearest_deadline = coder_deadline;
			pthread_mutex_lock(&simulation->mutex_print);
			printf("%ld %d burned out\n", time_in_ms, simulation->coders[i].id);
			pthread_cond_broadcast(&simulation->cond_monitor);
			pthread_mutex_unlock(&simulation->mutex_print);
			return (1);
		}
		i++;
	}
	return (0);
}
