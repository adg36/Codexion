/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:35:34 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/21 09:30:08 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void * arg)
{
	t_program		*simulation;
	long			nearest_burnout;
	long			remaining_ms;
	struct timespec	ts;
	long			time_in_ms;

	simulation = (t_program *)arg;

	pthread_mutex_lock(&simulation->mutex_monitor);
	nearest_burnout = find_nearest_burnout(simulation);
	while (1)
	{
		time_in_ms = get_timestamp(simulation->start);
		remaining_ms = nearest_burnout - time_in_ms;
		ts = build_deadline(remaining_ms);
		pthread_cond_timedwait(&simulation->cond_monitor, &simulation->mutex_monitor, &ts);
		if (burnout_detected(simulation))
			break ;
		else
		 	nearest_burnout = find_nearest_burnout(simulation);
	}
	pthread_mutex_unlock(&simulation->mutex_monitor);
	return (NULL);
}

int	burnout_detected(t_program *simulation)
{
	int		i;
	long	time_in_ms;
	int		coder_deadline;

	time_in_ms = get_timestamp(simulation->start);
	i = 0;
	while (i < simulation->settings.number_of_coders)
	{
		coder_deadline = simulation->coders[i].begin_of_last_compile + simulation->settings.time_to_burnout;
		if (time_in_ms > coder_deadline)
		{
			pthread_mutex_lock(&simulation->mutex_print);
			printf("%ld %d burned out\n", time_in_ms, simulation->coders[i].id);
			pthread_mutex_unlock(&simulation->mutex_print);
			return (1);
		}
		i++;
	}
	return (0);
}

long	find_nearest_burnout(t_program *simulation)
{
	int		i;
	int		nearest_deadline;
	int		coder_deadline;
	long	time_in_ms;

	time_in_ms = get_timestamp(simulation->start);
	i = 0;
	nearest_deadline = simulation->coders[0].begin_of_last_compile + simulation->settings.time_to_burnout;
	while (i < simulation->settings.number_of_coders)
	{
		coder_deadline = simulation->coders[i].begin_of_last_compile + simulation->settings.time_to_burnout;
		if (coder_deadline < nearest_deadline)
			nearest_deadline = coder_deadline;
		i++;
	}
	return (nearest_deadline);
}
