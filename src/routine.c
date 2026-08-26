/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/26 16:00:23 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *data)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)data;
	i = 0;
	while (i < coder->sim->settings.number_of_compiles_required && !coder->sim->stop_simulation)
	{
		// get in queue for both dongles
		if (strcmp(coder->sim->settings->scheduler, "fifo") == 0)
		{
			push(coder->dongle_l->queue, coder);
			push(coder->dongle_r->queue, coder);
		}
		else if (strcmp(coder->sim->settings->scheduler, "edf") == 0)
		{
			push_and_fix(coder->dongle_l->queue, coder);
			push_and_fix(coder->dongle_r->queue, coder);
		}
		if (!get_dongles(coder->sim->start, coder))
			return (NULL);
		else
		{
			compile(coder);
			if (all_compiles_completed(coder->sim))
				return (NULL);
			else
			{
				release_dongles(coder->sim->start, coder);
				print_logs(coder->sim->start, coder, "is debugging");
				usleep(coder->sim->settings.time_to_debug * 1000);
				print_logs(coder->sim->start, coder, "is refactoring");
				usleep(coder->sim->settings.time_to_refactor * 1000);
				i++;
			}
		}
	}
	return (NULL);
}

void	compile(t_coder *coder)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(coder->sim->start);
	coder->begin_of_last_compile = time_in_ms;
	print_logs(coder->sim->start, coder, "is compiling");
	pthread_cond_broadcast(&coder->sim->cond_monitor);
	usleep(coder->sim->settings.time_to_compile * 1000);
	pthread_mutex_lock(&coder->sim->mutex_compiles);
	coder->total_compiles++;
	pthread_mutex_unlock(&coder->sim->mutex_compiles);
}

void	print_logs(struct timeval start, t_coder *coder, char *message)
{
	long	time_in_ms;

	if (!coder->sim->stop_simulation)
	{
		time_in_ms = get_timestamp(start);
		pthread_mutex_lock(&coder->sim->mutex_print);
		printf("%ld %d %s\n", time_in_ms, coder->id, message);
		pthread_mutex_unlock(&coder->sim->mutex_print);
	}
}
