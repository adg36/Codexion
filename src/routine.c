/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/20 16:03:10 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *data)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)data;
	i = 0;
	while (i < coder->sim_data->settings.number_of_compiles_required && stop_simulation(coder->sim_data) == 0)
	{
		get_dongles(coder->sim_data->start, coder);
		compile(coder);
		release_dongles(coder->sim_data->start, coder);
		debug(coder->sim_data->start, coder->sim_data->settings.time_to_debug, coder);
		refactor(coder->sim_data->start, coder->sim_data->settings.time_to_refactor, coder);
		i++;
	}
	return (NULL);
}

void	compile(t_coder *coder)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(coder->sim_data->start);
	if (coder->dongle_left->held_by == coder->id && coder->dongle_right->held_by == coder->id)
	{
		pthread_mutex_lock(&coder->sim_data->mutex_print);
		coder->begin_of_last_compile = get_timestamp(coder->sim_data->start);
		coder->burnout = coder->begin_of_last_compile + coder->sim_data->settings.time_to_burnout;
		printf("%ld %d is compiling\n", time_in_ms, coder->id);
		pthread_mutex_unlock(&coder->sim_data->mutex_print);
		usleep(coder->sim_data->settings.time_to_compile * 1000);
	}
}

void	debug(struct timeval start, int time_to_debug, t_coder *coder)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	pthread_mutex_lock(&coder->sim_data->mutex_print);
	printf("%ld %d is debugging\n", time_in_ms, coder->id);
	pthread_mutex_unlock(&coder->sim_data->mutex_print);
	usleep(time_to_debug * 1000);
}

void	refactor(struct timeval start, int time_to_refactor, t_coder *coder)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	pthread_mutex_lock(&coder->sim_data->mutex_print);
	printf("%ld %d is refactoring\n", time_in_ms, coder->id);
	pthread_mutex_unlock(&coder->sim_data->mutex_print);
	usleep(time_to_refactor * 1000);
}

int	stop_simulation(t_program *simulation)
{
	int		i;
	long	time_in_ms;

	time_in_ms = get_timestamp(simulation->start);
	i = 0;
	while (i < simulation->settings.number_of_coders)
	{
		if (time_in_ms > simulation->coders[i].burnout)
		{
			printf("Coder %d burned out\n", simulation->coders[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}
