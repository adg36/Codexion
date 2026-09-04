/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:35:34 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/04 09:02:00 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void *arg)
{
	t_program		*sim;
	long			nearest_burnout;

	sim = (t_program *)arg;
	pthread_mutex_lock(&sim->mutex_monitor);
	nearest_burnout = find_nearest_burnout(sim);
	while (!simulation_stopped(sim) && !all_compiles_completed(sim))
	{
		wait_until_burnout(sim, nearest_burnout);
		if (burnout_detected(sim) || all_compiles_completed(sim))
		{
			stop_simulation(sim);
			break ;
		}
		else
			nearest_burnout = find_nearest_burnout(sim);
	}
	pthread_mutex_unlock(&sim->mutex_monitor);
	return (NULL);
}

int	wait_until_burnout(t_program *sim, long nearest_burnout)
{
	long			remaining_ms;
	struct timespec	ts;

	remaining_ms = nearest_burnout - get_timestamp(sim->start);
	ts = build_deadline(remaining_ms);
	return (pthread_cond_timedwait(&sim->cond_monitor,
			&sim->mutex_monitor, &ts));
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
		coder_deadline = simulation->coders[i].begin_of_last_compile
			+ simulation->settings.time_to_burnout;
		pthread_mutex_unlock(&simulation->mutex_compiles);
		if (time_in_ms > coder_deadline)
		{
			print_logs(simulation->start, &simulation->coders[i], "burned out");
			stop_simulation(simulation);
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
	nearest_deadline = simulation->coders[0].begin_of_last_compile
		+ simulation->settings.time_to_burnout;
	while (i < simulation->settings.number_of_coders)
	{
		coder_deadline = simulation->coders[i].begin_of_last_compile
			+ simulation->settings.time_to_burnout;
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

	if (simulation->settings.number_of_compiles_required == 0)
		return (1);
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
