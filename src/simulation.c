/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 14:19:17 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/03 15:25:53 by razevedo         ###   ########.fr       */
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
