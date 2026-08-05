/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:53:19 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/05 12:13:31 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

#include <bits/types/struct_timeval.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_settings
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_settings;

typedef struct s_simulation
{
	t_settings 		settings;
	pthread_mutex_t	mutex_dongles;
	pthread_t		*threads;
	struct timeval	start_time;
}	t_simulation;

typedef struct s_coder
{
	int	coder_id;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
}	t_coder;

typedef struct s_dongle
{
	int	dongle_id;
	int	dongle_cooldown;
}	t_dongle;

char	**get_args(int argc, char **argv);
int		are_args_valid(char **args, int len);
int		has_invalid_numbers(char **args, int len);
int		has_invalid_scheduler(char *str);
int		array_len(char **arr);
void	create_coders(t_settings *settings);
long	get_timestamp(struct timeval start);
void	compile(struct timeval start, int time_to_compile, int taskid);
void	debug(struct timeval start, int time_to_debug, int taskid);
void	refactor(struct timeval start, int time_to_refactor, int taskid);
void	init_settings(t_settings *settings, char **args);
void	init_coders(t_coder *coders, t_settings *settings);
void	init_simulation(t_simulation *simulation);
void	join_threads(t_settings *settings, pthread_t *threads);

#endif
