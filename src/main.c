/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:21:35 by razevedo          #+#    #+#             */
/*   Updated: 2026/07/23 18:02:19 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	char	**args;

	args = get_args(argc, argv);
	if (!args)
		return (0);
	if (!are_args_valid(args, array_len(args)))
	{
		write(2, "Error: invalid arguments.\n", 26);
		return (0);
	}
	for (int i = 0; i < 8; i++)
	{
		printf("%s\n", args[i]);
	}
}
