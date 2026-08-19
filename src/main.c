/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:52:49 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/19 09:04:57 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/pthreadtypes.h>
#include <complex.h>
#include <pthread.h>
#include <sys/types.h>

int	main(int argc, char **argv)
{
	char			**args;
	t_settings		settings;
	t_simulation	simulation;
	t_coder			*coders;
	t_dongle		*dongles;

	dongles = NULL;
	coders = NULL;
	args = get_args(argc, argv);
	if (!args)
		return (0);
	if (!are_args_valid(args, array_len(args)))
	{
		fprintf(stderr, "Error: invalid arguments.\n");
		return (0);
	}
	init_settings(&settings, args);
	dongles = init_dongles(dongles, &settings);
	coders = init_coders(coders, &settings, &simulation, dongles);
	init_sim(&simulation, &settings, coders, dongles);

	pthread_mutex_init(&simulation.mutex_dongles, NULL);
	pthread_cond_init(&simulation.cond_dongles, NULL);

	create_threads(&settings, &simulation, coders);

	join_threads(&settings, &simulation);

	pthread_mutex_destroy(&simulation.mutex_dongles);
	pthread_cond_destroy(&simulation.cond_dongles);

	return (0);
}

void	create_threads(t_settings *settings, t_simulation *simulation, t_coder *coders)
{
	int			i;

	simulation->threads = malloc(sizeof(pthread_t) * settings->number_of_coders);
	if (!simulation->threads)
		return ;
	i = 0;
	while (i < settings->number_of_coders)
	{
		if (pthread_create(&simulation->threads[i], NULL, routine, (void *) &coders[i]) != 0)
			fprintf(stderr, "Error creating thread %d\n", i);
		i++;
	}
	printf("%d threads created\n", settings->number_of_coders);
}

void	join_threads(t_settings *settings, t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < settings->number_of_coders)
	{
		if (pthread_join(simulation->threads[i], NULL) != 0)
			fprintf(stderr, "Error joining thread %d\n", i);
		i++;
	}
	printf("%d threads finished\n", settings->number_of_coders);
}

