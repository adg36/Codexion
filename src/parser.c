/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:53:33 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/01 13:58:37 by razevedo         ###   ########.fr       */
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
	if (atoi(args[0]) <= 0)
	{
		printf("There must be at least one coder.\n");
		return (0);
	}
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
