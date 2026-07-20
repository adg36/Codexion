/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:21:35 by razevedo          #+#    #+#             */
/*   Updated: 2026/07/20 15:31:36 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	args = handle_args(argc, argv);
	if (!args)
		return (0);
}

char	**handle_args(int argc, char **argv)
{
	char	**args;

	if (argc == 1)
		return (0);
	else
		args = argv + 1;
	return (args);
}

// maybe an extract_args function instead, which extracts directly each argument to the corresponding variable name:
// number_of_coders
// time_to_burnout (in milliseconds)
// time_to_compile (in milliseconds)
// time_to_debug (in milliseconds)
// time_to_refactor (in milliseconds)
// number_of_compiles_required
// dongle_cooldown (in milliseconds)
// scheduler (must be either fifo or edf)

// create function to validate arguments. All above arguments are mandatory.
// reject negative numbers, non-integers, invalid scheduler
