/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 17:18:40 by razevedo          #+#    #+#             */
/*   Updated: 2026/07/23 18:08:08 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

char	**get_args(int argc, char **argv)
{
	char	**args;

	if (argc != 9)
		return (0);
	else
		args = argv + 1;
	return (args);
}

int	are_args_valid(char **args, int len)
{
	if (has_invalid_numbers(args, len - 1))
		return (0);
	if (has_invalid_scheduler(args[7]))
		return (0);
	return (1);
}

int	has_invalid_numbers(char **args, int len)
{
	int	i;
	int	j;

	i = 0;
	while (i < len)
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] < '0' || args[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	has_invalid_scheduler(char *str)
{
	char	*s1;
	char	*s2;

	s1 = "fifo";
	s2 = "edf";
	if (strcmp(str, s1) == 0)
		return (0);
	if (strcmp(str, s2) == 0)
		return (0);
	return (1);
}

// check INT_MIN and INT_MAX??

// maybe an extract_args function instead, which extracts directly each argument to the corresponding variable name:
// number_of_coders
// time_to_burnout (in milliseconds)
// time_to_compile (in milliseconds)
// time_to_debug (in milliseconds)
// time_to_refactor (in milliseconds)
// number_of_compiles_required
// dongle_cooldown (in milliseconds)
// scheduler (must be either fifo or edf)
