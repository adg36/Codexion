/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/21 09:27:54 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *data)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)data;
	i = 0;
	while (i < coder->sim->settings.number_of_compiles_required)
	{
		get_dongles(coder->sim->start, coder);
		compile(coder);
		release_dongles(coder->sim->start, coder);
		debug(coder->sim->start, coder->sim->settings.time_to_debug, coder);
		refactor(coder->sim->start, coder->sim->settings.time_to_refactor, coder);
		i++;
	}
	return (NULL);
}

void	compile(t_coder *coder)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(coder->sim->start);
	if (coder->dongle_l->held_by == coder->id && coder->dongle_r->held_by == coder->id)
	{
		pthread_mutex_lock(&coder->sim->mutex_print);
		coder->begin_of_last_compile = get_timestamp(coder->sim->start);
		printf("%ld %d is compiling\n", time_in_ms, coder->id);
		pthread_cond_broadcast(&coder->sim->cond_monitor);
		pthread_mutex_unlock(&coder->sim->mutex_print);
		usleep(coder->sim->settings.time_to_compile * 1000);
	}
}

void	debug(struct timeval start, int time_to_debug, t_coder *coder)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	pthread_mutex_lock(&coder->sim->mutex_print);
	printf("%ld %d is debugging\n", time_in_ms, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex_print);
	usleep(time_to_debug * 1000);
}

void	refactor(struct timeval start, int time_to_refactor, t_coder *coder)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	pthread_mutex_lock(&coder->sim->mutex_print);
	printf("%ld %d is refactoring\n", time_in_ms, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex_print);
	usleep(time_to_refactor * 1000);
}
