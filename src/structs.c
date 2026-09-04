/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:19:28 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/04 14:28:48 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_settings(t_settings *settings, char **args)
{
	settings->number_of_coders = atoi(args[0]);
	settings->time_to_burnout = atoi(args[1]);
	settings->time_to_compile = atoi(args[2]);
	settings->time_to_debug = atoi(args[3]);
	settings->time_to_refactor = atoi(args[4]);
	settings->number_of_compiles_required = atoi(args[5]);
	settings->dongle_cooldown = atoi(args[6]);
	settings->scheduler = args[7];
}

t_dongle	*init_dongles(t_settings *settings)
{
	int			i;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * settings->number_of_coders);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < settings->number_of_coders)
	{
		dongles[i].id = i + 1;
		dongles[i].is_available = 1;
		dongles[i].never_used = 1;
		dongles[i].held_by = 0;
		dongles[i].began_cooldown = 0;
		dongles[i].queue = NULL;
		i++;
	}
	return (dongles);
}

t_coder	*init_coders(t_coder *coders, t_settings *settings,
	t_program *simulation, t_dongle *dongles)
{
	int	i;

	coders = malloc(sizeof(t_coder) * settings->number_of_coders);
	if (!coders)
		return (printf("Error: malloc failed.\n"), NULL);
	i = 0;
	while (i < settings->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].sim = simulation;
		set_dongles(i, settings, coders, dongles);
		coders[i].begin_of_last_compile = 0;
		coders[i].total_compiles = 0;
		coders[i].loss = 0;
		i++;
	}
	return (coders);
}

void	init_sim(t_program *sim, t_settings *settings,
			t_coder *coders, t_dongle *dongles)
{
	gettimeofday(&sim->start, NULL);
	sim->threads = NULL;
	sim->settings = *settings;
	sim->coders = coders;
	sim->dongles = dongles;
	sim->stop_simulation = 0;
	sim->threads_created = 0;
	sim->monitor_created = 0;
}

int	init_queues(t_settings *settings, t_dongle *dongles)
{
	int	i;

	i = 0;
	while (i < settings->number_of_coders)
	{
		dongles[i].queue = create_queue(QUEUE_CAPACITY);
		if (!dongles[i].queue)
		{
			i--;
			while (i >= 0)
			{
				free(dongles[i].queue->arr);
				free(dongles[i].queue);
				i--;
			}
			printf("Error: failed to create queue.\n");
			return (1);
		}
		i++;
	}
	return (0);
}
