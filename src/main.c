/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:52:49 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/07 13:26:51 by razevedo         ###   ########.fr       */
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
	coders = init_coders(coders, &settings);
	init_simulation(&simulation, &settings, coders);

	pthread_mutex_init(&simulation.mutex_dongles, NULL);

	create_threads(&settings, &simulation);

	join_threads(&settings, &simulation);

	pthread_mutex_destroy(&simulation.mutex_dongles);

	return (0);
}

void	create_threads(t_settings *settings, t_simulation *simulation)
{
	int			i;

	simulation->threads = malloc(sizeof(pthread_t) * settings->number_of_coders);
	if (!simulation->threads)
		return ;
	i = 0;
	while (i < settings->number_of_coders)
	{
		if (pthread_create(&simulation->threads[i], NULL, routine, (void *) simulation) != 0)
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
		if(pthread_join(simulation->threads[i], NULL) != 0)
			fprintf(stderr, "Error joining thread %d\n", i);
		i++;
	}
	printf("%d threads finished\n", settings->number_of_coders);
}

