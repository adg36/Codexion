/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/19 11:56:46 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

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
		debug(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_debug, coder_data->id);
		refactor(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_refactor, coder_data->id);
		i++;
	}
	return (NULL);
}

void	get_dongles(struct timeval start, t_coder *coder_data)
{
	long time_in_ms = 0;

	// printf("Start immediately before timestamp: %ld\n", time_in_ms);
	time_in_ms = get_timestamp(start);
	// printf("Time in ms immediately after timestamp: %ld\n", time_in_ms);

	pthread_mutex_lock(&coder_data->sim_data->mutex_dongles);
	while (coder_data->id != coder_data->dongle_left->held_by)
	{
		while (!coder_data->dongle_left->is_available)
		{
			printf("Left dongle unavailable for coder %d. Waiting.\n", coder_data->id);
			pthread_cond_wait(&coder_data->sim_data->cond_dongles, &coder_data->sim_data->mutex_dongles);
		}
		if (coder_data->dongle_left->never_used)
		{
			coder_data->dongle_left->is_available = 0;
			coder_data->dongle_left->held_by = coder_data->id;
			coder_data->dongle_left->never_used = 0;
			printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		}
		else if (time_in_ms > coder_data->dongle_left->began_cooldown + coder_data->sim_data->settings.dongle_cooldown)
		{
			coder_data->dongle_left->is_available = 0;
			coder_data->dongle_left->held_by = coder_data->id;
			printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		}
		pthread_mutex_unlock(&coder_data->sim_data->mutex_dongles);
	}
	pthread_mutex_lock(&coder_data->sim_data->mutex_dongles);
	while (coder_data->id != coder_data->dongle_right->held_by)
	{
		while (!coder_data->dongle_right->is_available)
		{
			printf("Right dongle unavailable for coder %d. Unlocking mutex and waiting to try again.\n", coder_data->id);
			pthread_cond_wait(&coder_data->sim_data->cond_dongles, &coder_data->sim_data->mutex_dongles);
		}
		if (coder_data->dongle_right->never_used)
		{
			coder_data->dongle_right->is_available = 0;
			coder_data->dongle_right->held_by = coder_data->id;
			coder_data->dongle_right->never_used = 0;
			printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		}
		else if (time_in_ms > coder_data->dongle_right->began_cooldown + coder_data->sim_data->settings.dongle_cooldown)
		{
			coder_data->dongle_right->is_available = 0;
			coder_data->dongle_right->held_by = coder_data->id;
			printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		}
		pthread_mutex_unlock(&coder_data->sim_data->mutex_dongles);
	}
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
	pthread_cond_signal(&coder_data->sim_data->cond_dongles);
	pthread_mutex_unlock(&coder_data->sim_data->mutex_dongles);
}

void	compile(t_coder *coder_data)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(coder_data->sim_data->start);
	if (coder_data->dongle_left->held_by == coder_data->id && coder_data->dongle_right->held_by == coder_data->id)
	{
		printf("%ld %d is compiling\n", time_in_ms, coder_data->id);
		usleep(coder_data->sim_data->settings.time_to_compile * 1000);
	}
}

void	debug(struct timeval start, int time_to_debug, int coder_id)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	printf("%ld %d is debugging\n", time_in_ms, coder_id);
	usleep(time_to_debug * 1000);
}

void	refactor(struct timeval start, int time_to_refactor, int coder_id)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	printf("%ld %d is refactoring\n", time_in_ms, coder_id);
	usleep(time_to_refactor * 1000);
}
