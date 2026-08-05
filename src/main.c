/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:52:49 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/05 14:37:11 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <bits/pthreadtypes.h>
#include <complex.h>
#include <pthread.h>
#include <sys/types.h>

// t_coder				thread_data_array[2];

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
	init_coders(coders, &settings);
	init_simulation(&simulation);

	pthread_mutex_init(&simulation.mutex_dongles, NULL);

	create_threads(&settings, &simulation);
	// pthread_create(&thread1, NULL, routine, (void *) &thread_data_array[0]);
	// pthread_create(&thread2, NULL, routine, (void *) &thread_data_array[1]);

	join_threads(&settings, &simulation);
	//pthread_join(thread1, NULL);
	//pthread_join(thread2, NULL);

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
		if (pthread_create(&simulation->threads[i], NULL, routine, (void *) &simulation) != 0)
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
	printf("%d threads joined\n", settings->number_of_coders);
}

