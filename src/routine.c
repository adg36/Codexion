/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/08 18:00:12 by razevedo         ###   ########.fr       */
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
		pthread_mutex_lock(&coder_data->sim_data->mutex_dongles);
		compile(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_compile, coder_data->id);
		pthread_mutex_unlock(&coder_data->sim_data->mutex_dongles);
		coder_data->dongle_left.began_cooldown = get_timestamp(coder_data->sim_data->start);
		coder_data->dongle_right.began_cooldown = get_timestamp(coder_data->sim_data->start);
		debug(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_debug, coder_data->id);
		refactor(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_refactor, coder_data->id);
		i++;
	}
	return (NULL);
}

void	get_dongles(struct timeval start, t_coder *coder_data)
{
	long time_in_ms;

	time_in_ms = get_timestamp(start);
	if (coder_data->dongle_left.began_cooldown > 0)
	{
		if (coder_data->dongle_left.began_cooldown + coder_data->sim_data->settings.dongle_cooldown > time_in_ms)
		{
			coder_data->dongle_left.is_available = 0;
			printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		}
		if (coder_data->dongle_right.began_cooldown + coder_data->sim_data->settings.dongle_cooldown > time_in_ms)
		{
			coder_data->dongle_right.is_available = 0;
			printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		}
	}
	else
	{
			coder_data->dongle_left.is_available = 0;
			printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
			coder_data->dongle_right.is_available = 0;
			printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
	}
}

void	compile(struct timeval start, int time_to_compile, int coder_id)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	printf("%ld %d is compiling\n", time_in_ms, coder_id);
	usleep(time_to_compile * 1000);
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