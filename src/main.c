/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:21:35 by razevedo          #+#    #+#             */
/*   Updated: 2026/07/29 16:23:44 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*compute()
{
	int	*result = malloc(sizeof(int));
	*result += 100;
	return result;
}

int	get_nb_of_coders(char **args)
{
	return (atoi(args[0]));
}

int	main(int argc, char **argv)
{
	char		**args;
	pthread_t	thread0;
	void		*return_value;
	int			number_of_coders;

	args = get_args(argc, argv);
	if (!args)
		return (0);
	if (!are_args_valid(args, array_len(args)))
	{
		write(2, "Error: invalid arguments.\n", 26);
		return (0);
	}
	
	number_of_coders = get_nb_of_coders(args);
	printf("Number of coders: %d\n", number_of_coders);
	
	printf("ARGS:\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%s\n", args[i]);
	}

	pthread_create(&thread0, NULL, compute, NULL);
	pthread_join(thread0, &return_value);

	int	*result = (int*) return_value;
	printf("Thread returned: %d\n", *result);

	free(result);
	return (0);
}