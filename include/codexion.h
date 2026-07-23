/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:34:57 by razevedo          #+#    #+#             */
/*   Updated: 2026/07/23 18:34:31 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

char	**get_args(int argc, char **argv);
int		are_args_valid(char **args, int len);
int		has_invalid_numbers(char **args, int len);
int		has_invalid_scheduler(char *str);
int		array_len(char **arr);

#endif
