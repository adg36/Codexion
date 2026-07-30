/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:21:35 by razevedo          #+#    #+#             */
/*   Updated: 2026/07/30 15:56:41 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

void	*routine()
{
	
	printf("[timestamp] [coder id] has taken a dongle.\n");
	printf("[timestamp] [coder id] has taken a dongle.\n");
	printf("[coder id] is compiling\n");
	printf("[coder id] is debugging\n");
	printf("[coder id] is refactoring\n");
	
	return NULL;
}

int	main(int argc, char **argv)
{
	char		**args;
	int			number_of_coders;
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
	
	/*
	create_coders(number_of_coders);
			
	printf("ARGS:\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%s\n", args[i]);
	}
	*/

	pthread_create(&thread1, NULL, routine, NULL);
	pthread_create(&thread2, NULL, routine, NULL);
	

	// Wait for threads to complete
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	// complete_threads(number_of_coders, threads);
	
	return (0);
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