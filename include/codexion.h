/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:53:19 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/28 16:26:39 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define QUEUE_CAPACITY 2

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_program	t_program;
typedef struct s_coder		t_coder;

typedef struct	s_queue
{
	int	*array;
	int	size;
	int	capacity;
}	t_queue;

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
	t_queue	*queue;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	t_program	*sim;
	t_dongle	*first_dongle;
	t_dongle	*second_dongle;
	long		begin_of_last_compile;
	int			total_compiles;
}	t_coder;

typedef struct s_program
{
	t_settings		settings;
	pthread_mutex_t	mutex_dongles;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_monitor;
	pthread_mutex_t	mutex_compiles;
	pthread_cond_t	cond_dongles;
	pthread_cond_t	cond_monitor;
	t_dongle		*dongles;
	pthread_t		*threads;
	pthread_t		monitor;
	int				stop_simulation;
	struct timeval	start;
	t_coder			*coders;
}	t_program;

char			**get_args(int argc, char **argv);
int				are_args_valid(char **args, int len);
int				has_invalid_numbers(char **args, int len);
int				has_invalid_scheduler(char *str);
int				array_len(char **arr);
int				create_threads(
					t_settings *settings,
					t_program *simulation,
					t_coder *coders);
int				join_threads(t_settings *settings, t_program *simulation);
long			get_timestamp(struct timeval start);
struct timespec	build_deadline(long remaining_ms);
// int				dongles_are_unavailable(struct timeval start, t_coder *data);
int				get_dongles(struct timeval start, t_coder *data);
// void			assign_dongles(t_coder *coder);
void			compile(t_coder *data);
void			release_dongles(struct timeval start, t_coder *data);
void			debug(struct timeval start, int time_to_debug, t_coder *data);
void			refactor(
					struct timeval start,
					int time_to_refactor,
					t_coder *data);
void			init_settings(t_settings *settings, char **args);
t_coder			*init_coders(
					t_coder *coders,
					t_settings *settings,
					t_program *simulation,
					t_dongle *dongles);
t_dongle		*init_dongles(t_dongle *dongles, t_settings *settings);
void			init_sim(
					t_program *sim,
					t_settings *settings,
					t_coder *coders,
					t_dongle *dongles);
void			init_mutex_cond(t_program *simulation);
void			init_queues(t_settings *settings, t_dongle *dongles);
t_queue			*create_queue(int capacity);
void			destroy_mutex_cond(t_program *simulation);
void			*routine(void *data);
void			*monitor(void *arg);
int				burnout_detected(t_program *simulation);
long			find_nearest_burnout(t_program *simulation);
int				all_compiles_completed(t_program *simulation);
void			print_logs(struct timeval start, t_coder *coder, char *message);
void			swap(int *a, int *b);
void			push_and_fix(t_queue *queue, t_settings *settings, t_coder *coder);
void			push(t_queue *queue, t_coder *coder);
int				pop_left(t_queue *queue);
int				pop_right(t_queue *queue);
int				first_in_line(t_queue *queue);
int				has_priority(t_coder *coder, t_dongle *dongle);
int				first_dongle_is_unavailable(struct timeval start, t_coder *coder);
int				second_dongle_is_unavailable(struct timeval start, t_coder *coder);
void			grab_dongle(t_coder *coder, t_dongle *dongle);

#endif
