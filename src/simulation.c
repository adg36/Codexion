/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 14:19:17 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/04 09:21:10 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_program(t_settings *settings, t_program *simulation)
{
	t_coder		*coders;
	t_dongle	*dongles;

	coders = NULL;
	dongles = init_dongles(settings);
	if (!dongles)
		return (fprintf(stderr, "Error: failed to create dongles.\n"), 1);
	coders = init_coders(coders, settings, simulation, dongles);
	if (!coders)
	{
		free(simulation->dongles);
		return (fprintf(stderr, "Error: failed to create coders.\n"), 1);
	}
	init_queues(settings, dongles);
	init_sim(simulation, settings, coders, dongles);
	init_mutex_cond(simulation);
	pre_enqueue(simulation);
	return (0);
}

void	stop_simulation(t_program *simulation)
{
	pthread_mutex_lock(&simulation->mutex_sim);
	simulation->stop_simulation = 1;
	pthread_mutex_unlock(&simulation->mutex_sim);
}

int	simulation_stopped(t_program *simulation)
{
	int	stop;

	pthread_mutex_lock(&simulation->mutex_sim);
	stop = simulation->stop_simulation;
	pthread_mutex_unlock(&simulation->mutex_sim);
	return (stop);
}

int	wait_for_dongles(struct timeval start, t_coder *coder)
{
	long			remaining_ms;
	long			remaining_ms1;
	long			remaining_ms2;
	struct timespec	ts;

	if (!coder->first_dongle->is_available)
	{
		remaining_ms = get_remaining_ms(start, coder, coder->first_dongle);
		ts = build_deadline(remaining_ms);
	}
	else if (!coder->second_dongle->is_available)
	{
		remaining_ms = get_remaining_ms(start, coder, coder->second_dongle);
		ts = build_deadline(remaining_ms);
	}
	else
	{
		remaining_ms1 = coder->first_dongle->began_cooldown
			+ coder->sim->settings.dongle_cooldown - get_timestamp(start);
		remaining_ms2 = coder->second_dongle->began_cooldown
			+ coder->sim->settings.dongle_cooldown - get_timestamp(start);
		ts = build_deadline(later_deadline(remaining_ms1, remaining_ms2));
	}
	return (pthread_cond_timedwait(&coder->sim->cond_dongles,
			&coder->sim->mutex_dongles, &ts));
}

long	get_remaining_ms(struct timeval start, t_coder *coder, t_dongle *dongle)
{
	long	remaining_ms;
	long	now;

	now = get_timestamp(start);
	pthread_mutex_lock(&coder->sim->mutex_compiles);
	remaining_ms = coder->sim->coders[dongle->held_by - 1].begin_of_last_compile
		+ coder->sim->settings.time_to_compile
		+ coder->sim->settings.dongle_cooldown - now;
	pthread_mutex_unlock(&coder->sim->mutex_compiles);
	return (remaining_ms);
}
