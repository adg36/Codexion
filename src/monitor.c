/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:35:34 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/31 14:18:50 by razevedo         ###   ########.fr       */
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
			pthread_mutex_lock(&simulation->mutex_sim);
			simulation->stop_simulation = 1;
			pthread_mutex_unlock(&simulation->mutex_sim);
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
		pthread_mutex_lock(&simulation->mutex_compiles);
		coder_deadline = simulation->coders[i].begin_of_last_compile + simulation->settings.time_to_burnout;
		pthread_mutex_unlock(&simulation->mutex_compiles);
		if (time_in_ms > coder_deadline)
		{
			print_logs(simulation->start, &simulation->coders[i], "burned out");
			pthread_mutex_lock(&simulation->mutex_sim);
			simulation->stop_simulation = 1;
			pthread_mutex_unlock(&simulation->mutex_sim);
			pthread_mutex_lock(&simulation->mutex_dongles);
			pthread_cond_broadcast(&simulation->cond_dongles);
			pthread_mutex_unlock(&simulation->mutex_dongles);
			pthread_cond_broadcast(&simulation->cond_monitor);
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
	pthread_mutex_lock(&simulation->mutex_compiles);
	nearest_deadline = simulation->coders[0].begin_of_last_compile + simulation->settings.time_to_burnout;
	while (i < simulation->settings.number_of_coders)
	{
		coder_deadline = simulation->coders[i].begin_of_last_compile + simulation->settings.time_to_burnout;
		if (coder_deadline < nearest_deadline)
			nearest_deadline = coder_deadline;
		i++;
	}
	pthread_mutex_unlock(&simulation->mutex_compiles);
	return (nearest_deadline);
}

int	all_compiles_completed(t_program *simulation)
{
	int	i;
	int	total_comp;

	i = 0;
	while (i < simulation->settings.number_of_coders)
	{
		pthread_mutex_lock(&simulation->mutex_compiles);
		total_comp = simulation->coders[i].total_compiles;
		pthread_mutex_unlock(&simulation->mutex_compiles);
		if (total_comp < simulation->settings.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}
