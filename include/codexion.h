/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:53:19 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/20 15:51:41 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <errno.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_program t_program;

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

typedef struct s_dongle
{
	int		id;
	int		is_available;
	int		held_by;
	int		never_used;
	long	began_cooldown;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	t_program	*sim_data;
	t_dongle	*dongle_left;
	t_dongle	*dongle_right;
	long		begin_of_last_compile;
	long		burnout;
}	t_coder;

typedef struct s_program
{
	t_settings		settings;
	pthread_mutex_t	mutex_dongles;
	pthread_mutex_t	mutex_print;
	pthread_cond_t	cond_dongles;
	t_dongle		*dongles; // [dongle1, dongle2, etc.]
	pthread_t		*threads; // [thread1, thread2, etc.]
	pthread_t		monitor;
	struct timeval	start;
	t_coder			*coders; // [coder1, coder2, etc.]
}	t_program;

char			**get_args(int argc, char **argv);
int				are_args_valid(char **args, int len);
int				has_invalid_numbers(char **args, int len);
int				has_invalid_scheduler(char *str);
int				array_len(char **arr);
int				create_threads(t_settings *settings, t_program *simulation, t_coder *coders);
int				join_threads(t_settings *settings, t_program *simulation);
long			get_timestamp(struct timeval start);
struct timespec build_deadline(long remaining_ms);
int				dongles_are_unavailable(struct timeval start, t_coder *data);
void			get_dongles(struct timeval start, t_coder *data);
void			compile(t_coder *data);
void			release_dongles(struct timeval start, t_coder *data);
void			debug(struct timeval start, int time_to_debug, t_coder *data);
void			refactor(struct timeval start, int time_to_refactor, t_coder *data);
void			init_settings(t_settings *settings, char **args);
t_coder			*init_coders(t_coder *coders, t_settings *settings, t_program *simulation, t_dongle *dongles);
t_dongle		*init_dongles(t_dongle *dongles, t_settings *settings);
void			init_sim(t_program *sim, t_settings *settings, t_coder *coders, t_dongle *dongles);
void			*routine(void *data);
void			*monitor();
int				stop_simulation(t_program *simulation);


#endif
