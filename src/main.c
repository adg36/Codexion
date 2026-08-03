/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:21:35 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/03 14:26:32 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

Coder	thread_data_array[2];

void	*routine(void *threadarg)
{
	Coder				*my_data;
	int					taskid;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	struct timeval		start;
	long				time_in_ms;

	my_data = (Coder *) threadarg;
	taskid = my_data->id;
	time_to_compile = my_data->time_to_compile;
	time_to_debug = my_data->time_to_debug;
	time_to_refactor = my_data->time_to_refactor;	

	gettimeofday(&start, NULL);
	time_in_ms = get_timestamp(start);
	printf("%ld %d has taken a dongle.\n", time_in_ms, taskid);
	time_in_ms = get_timestamp(start);
	printf("%ld %d has taken a dongle.\n", time_in_ms, taskid);
	time_in_ms = get_timestamp(start);
	printf("%ld %d is compiling\n", time_in_ms, taskid);
	usleep(time_to_compile * 1000);
	time_in_ms = get_timestamp(start);
	printf("%ld %d is debugging\n", time_in_ms, taskid);
	usleep(time_to_debug * 1000);
	time_in_ms = get_timestamp(start);
	printf("%ld %d is refactoring\n", time_in_ms, taskid);
	usleep(time_to_refactor * 1000);
	
	return NULL;
}

int	main(int argc, char **argv)
{
	char		**args;
	int			number_of_coders;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	pthread_t	thread1;
	pthread_t	thread2;

	args = get_args(argc, argv);
	if (!args)
		return (0);
	if (!are_args_valid(args, array_len(args)))
	{
		write(2, "Error: invalid arguments.\n", 26);
		return (0);
	}
	
	number_of_coders = atoi(args[0]);
	printf("Number of coders: %d\n", number_of_coders);
	time_to_compile = atoi(args[2]);
	printf("Time to compile: %d\n", time_to_compile);
	time_to_debug = atoi(args[3]);
	printf("Time to debug: %d\n", time_to_debug);
	time_to_refactor = atoi(args[4]);
	printf("Time to refactor: %d\n", time_to_refactor);
	
	/*
	create_coders(number_of_coders);
			
	printf("ARGS:\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%s\n", args[i]);
	}
	*/

	thread_data_array[0].id = 1;
	thread_data_array[1].id = 2;
	thread_data_array[0].time_to_compile = time_to_compile;
	thread_data_array[1].time_to_compile = time_to_compile;
	thread_data_array[0].time_to_debug = time_to_debug;
	thread_data_array[1].time_to_debug = time_to_debug;
	thread_data_array[0].time_to_refactor = time_to_refactor;
	thread_data_array[1].time_to_refactor = time_to_refactor;
	
	pthread_create(&thread1, NULL, routine, (void *) &thread_data_array[0]);
	pthread_create(&thread2, NULL, routine, (void *) &thread_data_array[1]);
	

	// Wait for threads to complete
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	// complete_threads(number_of_coders, threads);
	
	return (0);
}

long	get_timestamp(struct timeval start)
{
	struct timeval		end;
	long				seconds;
	long				microseconds;
	long				total_time;
	long				time_in_ms;
	
	gettimeofday(&end, NULL);
	seconds = end.tv_sec - start.tv_sec;
	microseconds = end.tv_usec - start.tv_usec;
	total_time = seconds * 1000000 + microseconds;
	time_in_ms = total_time / 1000;

	return time_in_ms;
}

void	create_coders(int number_of_coders)
{
	int	i;
	pthread_t threads[number_of_coders];
	
	// Create threads
	i = 0;
	while (i < number_of_coders)
	{
		if (pthread_create(&threads[i], NULL, routine, NULL) != 0)
			fprintf(stderr, "Error creating thread %d\n", i);
		i++;
	}
	printf("%d threads created\n", number_of_coders);
}