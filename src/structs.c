/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:19:28 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/21 08:48:54 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/types/struct_timeval.h>
#include <pthread.h>

void	init_settings(t_settings *settings, char **args)
{
	settings->number_of_coders = atoi(args[0]);
	printf("Number of coders: %d\n", settings->number_of_coders);
	settings->time_to_burnout = atoi(args[1]);
	printf("Time to burnout: %d\n", settings->time_to_burnout);
	settings->time_to_compile = atoi(args[2]);
	printf("Time to compile: %d\n", settings->time_to_compile);
	settings->time_to_debug = atoi(args[3]);
	printf("Time to debug: %d\n", settings->time_to_debug);
	settings->time_to_refactor = atoi(args[4]);
	printf("Time to refactor: %d\n", settings->time_to_refactor);
	settings->number_of_compiles_required = atoi(args[5]);
	printf("Number of compiles required: %d\n", settings->number_of_compiles_required);
	settings->dongle_cooldown = atoi(args[6]);
	printf("Dongle cooldown: %d\n", settings->dongle_cooldown);
	settings->scheduler = args[7];
	printf("Scheduler: %s\n", settings->scheduler);
}

t_dongle	*init_dongles(t_dongle *dongles, t_settings *settings)
{
	int	i;

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
		i++;
	}
	printf("%d dongles created\n", settings->number_of_coders);
	return (dongles);
}

t_coder	*init_coders(t_coder *coders, t_settings *settings, t_program *simulation, t_dongle *dongles)
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
		coders[i].dongle_l = &(dongles[i]);
		coders[i].dongle_r = &(dongles[(i + 1) % settings->number_of_coders]);
		coders[i].begin_of_last_compile = 0;
		coders[i].burnout = 0;
		i++;
	}
	printf("%d coders (structs) created\n", settings->number_of_coders);
	return (coders);
}

void	init_sim(t_program *sim, t_settings *settings, t_coder *coders, t_dongle *dongles)
{
	gettimeofday(&sim->start, NULL);
	sim->threads = NULL;
	sim->settings = *settings;
	sim->coders = coders;
	sim->dongles = dongles;

	printf("Start of the simulation at %ld\n", (long)sim->start.tv_sec);
}
