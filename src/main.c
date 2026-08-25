/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:52:49 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/25 12:10:43 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	char			**args;
	t_settings		settings;
	t_program		simulation;
	t_coder			*coders;
	t_dongle		*dongles;

	if (argc != 9)
		return (fprintf(stderr, "Error: wrong number of arguments.\n"), 1);
	args = get_args(argc, argv);
	if (!args)
		return (fprintf(stderr, "Error: parsing error.\n"), 1);
	if (!are_args_valid(args, array_len(args)))
		return (fprintf(stderr, "Error: invalid arguments.\n"), 1);
	init_settings(&settings, args);
	dongles = NULL;
	coders = NULL;
	dongles = init_dongles(dongles, &settings);
	if (!dongles)
	{
		fprintf(stderr, "Error: failed to create dongles.\n");
		return (1);
	}
	coders = init_coders(coders, &settings, &simulation, dongles);
	if (!coders)
	{
		free(simulation.dongles);
		fprintf(stderr, "Error: failed to create coders.\n");
		return (2);
	}
	init_sim(&simulation, &settings, coders, dongles);

	pthread_mutex_init(&simulation.mutex_dongles, NULL);
	pthread_mutex_init(&simulation.mutex_print, NULL);
	pthread_mutex_init(&simulation.mutex_monitor, NULL);
	pthread_mutex_init(&simulation.mutex_compiles, NULL);
	pthread_cond_init(&simulation.cond_dongles, NULL);
	pthread_cond_init(&simulation.cond_monitor, NULL);

	create_threads(&settings, &simulation, coders);

	join_threads(&settings, &simulation);

	pthread_mutex_destroy(&simulation.mutex_dongles);
	pthread_mutex_destroy(&simulation.mutex_print);
	pthread_mutex_destroy(&simulation.mutex_monitor);
	pthread_mutex_destroy(&simulation.mutex_compiles);
	pthread_cond_destroy(&simulation.cond_dongles);
	pthread_cond_destroy(&simulation.cond_monitor);

	free(simulation.dongles);
	free(simulation.coders);
	free(simulation.threads);
	return (0);
}

int	create_threads(t_settings *settings, t_program *simulation, t_coder *coders)
{
	int			i;

	simulation->threads = malloc(sizeof(pthread_t) * settings->number_of_coders);
	if (!simulation->threads)
		return (fprintf(stderr, "Error: failed to allocate threads.\n"), 1);
	i = 0;
	while (i < settings->number_of_coders)
	{
		if (pthread_create(&simulation->threads[i], NULL, routine, (void *) &coders[i]) != 0)
			return (fprintf(stderr, "Error creating thread %d\n", i), 1);
		i++;
	}
	if (pthread_create(&simulation->monitor, NULL, monitor, (void *) simulation) != 0)
		return (fprintf(stderr, "Error creating monitor thread.\n"), 1);
	return (0);
}

int	join_threads(t_settings *settings, t_program *simulation)
{
	int	i;

	i = 0;
	while (i < settings->number_of_coders)
	{
		if (pthread_join(simulation->threads[i], NULL) != 0)
			return (fprintf(stderr, "Error joining thread %d\n", i), 1);
		i++;
	}
	pthread_join(simulation->monitor, NULL);
	return (0);
}

