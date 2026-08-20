/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/20 13:27:39 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *threadarg)
{
	t_coder	*coder_data;
	int		i;

	coder_data = (t_coder *) threadarg;
	i = 0;
	while (i < coder_data->sim_data->settings.number_of_compiles_required)
	{
		get_dongles(coder_data->sim_data->start, coder_data);
		compile(coder_data);
		release_dongles(coder_data->sim_data->start, coder_data);
		debug(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_debug, coder_data);
		refactor(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_refactor, coder_data);
		i++;
	}
	return (NULL);
}

void	get_dongles(struct timeval start, t_coder *coder_data)
{
	long			time_in_ms;
	long			remaining_ms;
	struct timespec	ts;
	int				rc;

	pthread_mutex_lock(&coder_data->sim_data->mutex_dongles);
	while (coder_data->id != coder_data->dongle_left->held_by && coder_data->id != coder_data->dongle_right->held_by)
	{
		while ((!coder_data->dongle_left->is_available
			|| (!coder_data->dongle_left->never_used
			&& get_timestamp(start) <= coder_data->dongle_left->began_cooldown + coder_data->sim_data->settings.dongle_cooldown))
			|| (!coder_data->dongle_right->is_available
			|| (!coder_data->dongle_right->never_used
			&& get_timestamp(start) <= coder_data->dongle_right->began_cooldown + coder_data->sim_data->settings.dongle_cooldown))
			|| coder_data->dongle_left == coder_data->dongle_right)
		{
			time_in_ms = get_timestamp(start);
			remaining_ms = coder_data->dongle_left->began_cooldown + coder_data->sim_data->settings.dongle_cooldown - time_in_ms;
			ts = build_deadline(remaining_ms);
			// printf("Left dongle unavailable for coder %d. Waiting.\n", coder_data->id);
			rc = pthread_cond_timedwait(&coder_data->sim_data->cond_dongles,
				&coder_data->sim_data->mutex_dongles, &ts);
			(void)rc;
		}
		coder_data->dongle_left->is_available = 0;
		coder_data->dongle_right->is_available = 0;
		coder_data->dongle_left->held_by = coder_data->id;
		coder_data->dongle_right->held_by = coder_data->id;
		coder_data->dongle_left->never_used = 0;
		coder_data->dongle_right->never_used = 0;
		time_in_ms = get_timestamp(start);
		pthread_mutex_lock(&coder_data->sim_data->mutex_print);
		printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		pthread_mutex_unlock(&coder_data->sim_data->mutex_print);
	}
	pthread_mutex_unlock(&coder_data->sim_data->mutex_dongles);
}

void	release_dongles(struct timeval start, t_coder *coder_data)
{
	pthread_mutex_lock(&coder_data->sim_data->mutex_dongles);
	if (coder_data->id == coder_data->dongle_left->held_by && coder_data->id == coder_data->dongle_right->held_by)
	{
		coder_data->dongle_left->is_available = 1;
		coder_data->dongle_left->held_by = 0;
		coder_data->dongle_right->is_available = 1;
		coder_data->dongle_right->held_by = 0;
		coder_data->dongle_left->began_cooldown = get_timestamp(start);
		coder_data->dongle_right->began_cooldown = get_timestamp(start);
	}
	pthread_cond_broadcast(&coder_data->sim_data->cond_dongles);
	pthread_mutex_unlock(&coder_data->sim_data->mutex_dongles);
}

void	compile(t_coder *coder_data)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(coder_data->sim_data->start);
	if (coder_data->dongle_left->held_by == coder_data->id && coder_data->dongle_right->held_by == coder_data->id)
	{
		pthread_mutex_lock(&coder_data->sim_data->mutex_print);
		printf("%ld %d is compiling\n", time_in_ms, coder_data->id);
		pthread_mutex_unlock(&coder_data->sim_data->mutex_print);
		usleep(coder_data->sim_data->settings.time_to_compile * 1000);
	}
}

void	debug(struct timeval start, int time_to_debug, t_coder *coder_data)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	pthread_mutex_lock(&coder_data->sim_data->mutex_print);
	printf("%ld %d is debugging\n", time_in_ms, coder_data->id);
	pthread_mutex_unlock(&coder_data->sim_data->mutex_print);
	usleep(time_to_debug * 1000);
}

void	refactor(struct timeval start, int time_to_refactor, t_coder *coder_data)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	pthread_mutex_lock(&coder_data->sim_data->mutex_print);
	printf("%ld %d is refactoring\n", time_in_ms, coder_data->id);
	pthread_mutex_unlock(&coder_data->sim_data->mutex_print);
	usleep(time_to_refactor * 1000);
}
