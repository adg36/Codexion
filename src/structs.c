/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:19:28 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/05 14:42:47 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"
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

void	init_coders(t_coder *coders, t_settings *settings)
{
	int	i;

	coders = malloc(sizeof(t_coder) * settings->number_of_coders);

	if (!coders)
		return ;

	i = 0;
	while (i < settings->number_of_coders)
	{
		coders[i].coder_id = i + 1;
		i++;
	}
	printf("%d coders (structs) created\n", settings->number_of_coders);
}

void	init_simulation(t_simulation *simulation)
{
	gettimeofday(&simulation->start, NULL);
	simulation->threads = NULL;

	printf("Start of the simulation at %ld\n", (long)simulation->start.tv_sec);
}
