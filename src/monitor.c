/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:35:34 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/29 08:55:05 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void *arg)
{
	t_program		*simulation;
	long			nearest_burnout;
	long			remaining_ms;
	struct timespec	ts;
	int				rc;
	long			time_in_ms;

	simulation = (t_program *)arg;
	pthread_mutex_lock(&simulation->mutex_monitor);
	nearest_burnout = find_nearest_burnout(simulation);
	while (1)
	{
		time_in_ms = get_timestamp(simulation->start);
		remaining_ms = nearest_burnout - time_in_ms;
		ts = build_deadline(remaining_ms);
		rc = pthread_cond_timedwait(&simulation->cond_monitor, &simulation->mutex_monitor, &ts);
		if (burnout_detected(simulation) || all_compiles_completed(simulation))
		{
			simulation->stop_simulation = 1;
			break ;
		}
		else
			nearest_burnout = find_nearest_burnout(simulation);
	}
	pthread_mutex_unlock(&simulation->mutex_monitor);
	(void)rc;
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
			pthread_cond_broadcast(&simulation->cond_dongles);
			pthread_cond_broadcast(&simulation->cond_monitor);
			print_logs(simulation->start, &simulation->coders[i], "burned out");
			simulation->stop_simulation = 1;
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

int	all_compiles_completed(t_program *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->settings.number_of_coders)
	{
		if (simulation->coders[i].total_compiles < simulation->settings.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}
