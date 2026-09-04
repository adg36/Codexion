/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:52:49 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/04 14:44:57 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	char			**args;
	t_settings		settings;
	t_program		simulation;

	if (argc != 9)
		return (fprintf(stderr, "Error: wrong number of arguments.\n"), 1);
	args = argv + 1;
	if (!are_args_valid(args))
		return (fprintf(stderr, "Error: invalid arguments.\n"), 1);
	init_settings(&settings, args);
	if (init_program(&settings, &simulation))
		return (fprintf(stderr, "Error initializing the simulation.\n"), 1);
	if (create_threads(&settings, &simulation))
	{
		join_threads(&simulation);
		clean_up(&settings, &simulation);
		return (fprintf(stderr, "Error: failed to create threads.\n"), 1);
	}
	join_threads(&simulation);
	clean_up(&settings, &simulation);
	return (0);
}

int	create_threads(t_settings *settings, t_program *simulation)
{
	int	i;

	simulation->threads = (
			malloc(sizeof(pthread_t) * settings->number_of_coders));
	if (!simulation->threads)
		return (1);
	i = 0;
	while (i < settings->number_of_coders)
	{
		if (pthread_create(&simulation->threads[i], NULL,
				routine, (void *) &simulation->coders[i]) != 0)
		{
			simulation->threads_created = i;
			return (1);
		}
		i++;
		simulation->threads_created++;
	}
	if (pthread_create(
			&simulation->monitor, NULL, monitor, (void *) simulation) != 0)
		return (stop_simulation(simulation), 1);
	simulation->monitor_created = 1;
	return (0);
}

void	join_threads(t_program *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->threads_created)
	{
		pthread_join(simulation->threads[i], NULL);
		i++;
	}
	if (simulation->monitor_created == 1)
		pthread_join(simulation->monitor, NULL);
}

void	free_queues(t_settings *settings, t_dongle *dongles)
{
	int	i;

	i = 0;
	while (i < settings->number_of_coders)
	{
		free(dongles[i].queue->arr);
		free(dongles[i].queue);
		i++;
	}
}

void	clean_up(t_settings *settings, t_program *simulation)
{
	destroy_mutex_cond(simulation);
	free_queues(settings, simulation->dongles);
	free(simulation->dongles);
	free(simulation->coders);
	free(simulation->threads);
}
